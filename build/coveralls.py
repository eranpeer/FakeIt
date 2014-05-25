from __future__ import absolute_import
from __future__ import print_function

import argparse
from context import *

import requests
import json

import os
import subprocess


def gitrepo(cwd):
    """Return hash of Git data that can be used to display more information to
    users.

    Example:
        "git": {
            "head": {
                "id": "5e837ce92220be64821128a70f6093f836dd2c05",
                "author_name": "Wil Gieseler",
                "author_email": "wil@example.com",
                "committer_name": "Wil Gieseler",
                "committer_email": "wil@example.com",
                "message": "depend on simplecov >= 0.7"
            },
            "branch": "master",
            "remotes": [{
                "name": "origin",
                "url": "https://github.com/lemurheavy/coveralls-ruby.git"
            }]
        }

    From https://github.com/coagulant/coveralls-python (with MIT license).
    """
    repo = Repository(cwd)

    return {
        'head': {
            'id': repo.gitlog('%H'),
            'author_name': repo.gitlog('%aN'),
            'author_email': repo.gitlog('%ae'),
            'committer_name': repo.gitlog('%cN'),
            'committer_email': repo.gitlog('%ce'),
            'message': repo.gitlog('%s')
        },
        'branch': os.environ.get('TRAVIS_BRANCH', repo.git(
            'rev-parse', '--abbrev-ref', 'HEAD').strip()),
        'remotes': [{'name': line.split()[0], 'url': line.split()[1]}
                    for line in repo.git('remote', '-v') if '(fetch)' in line]
    }


class Repository(object):

    def __init__(self, cwd):
        self.cwd = cwd

    def gitlog(self, fmt):
        return self.git('--no-pager', 'log', '-1', '--pretty=format:%s' % fmt)

    def git(self, *arguments):
        """Return output from git."""
        process = subprocess.Popen(['git'] + list(arguments),
                                   stdout=subprocess.PIPE,
                                   cwd=self.cwd)
        return process.communicate()[0].decode('UTF-8')

URL = 'https://coveralls.io/api/v1/jobs'

def post_report(coverage_report):
    response = requests.post(URL, files={'json_file': json.dumps(coverage_report)})
    try:
        result = response.json()
    except ValueError:
        result = {'error': 'Failure to submit data. '
                  'Response [%(status)s]: %(text)s' % {
                      'status': response.status_code,
                      'text': response.text}}
    print(result)
    if 'error' in result:
        return result['error']
    return 0

def create_args(params):
    parser = argparse.ArgumentParser('coveralls')
    parser.add_argument('-r', '--root', metavar='DIR', default='.',
                        help='set the root directory')
    parser.add_argument('-b', '--build-root', metavar='DIR', default='.',
                        help='the directory used as root for building the source')
    parser.add_argument('-e', '--exclude', metavar='DIR|FILE',
                        dest='exclude_list', action='append', default=[],
                        help='set exclude file or directory')
    parser.add_argument('-E', '--exclude-pattern', dest='exclude_patterns',
                        action='append', metavar='REGEXP', default=[],
                        help='set exclude file/directory pattern')
    parser.add_argument('-x', '--extension', metavar='EXT', action='append',
                        help='set extension of files to process')
    parser.add_argument('-t', '--repo-token', '--repo_token', default='',
                        metavar='TOKEN',
                        help='set the repo_token of this project')
    parser.add_argument('--encoding', default='utf-8',
                        help='source encoding (default: %(default)s)')
    parser.add_argument('--verbose', action='store_true',
                        help='print verbose messages')
    return parser.parse_args(params)


def run():
    import os
    import sys

    args = create_args(sys.argv[1:])

    context = Context(args.root,args.build_root, args.exclude_list, args.exclude_patterns, args.encoding)

    args.repo_token = ''
    args.service_name = 'travis-ci'
    args.service_job_id = os.environ.get('TRAVIS_JOB_ID', '')

    cov_report = {}
    if args.repo_token:
        cov_report['repo_token'] = args.repo_token
    cov_report['service_name'] = args.service_name
    cov_report['service_job_id'] = args.service_job_id

    source_reports = context.make_source_reports()

    cov_report['source_files'] = source_reports;
    cov_report['git'] = gitrepo('.')

    if args.verbose:
        print(cov_report)
    return post_report(cov_report)

if __name__ == "__main__":
    run()