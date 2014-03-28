from __future__ import absolute_import
from __future__ import print_function

__author__ = 'Lei Xu <eddyxu@gmail.com>'
__version__ = '0.1.5'

__classifiers__ = [
    'Development Status :: 2 - Pre-Alpha',
    'Intended Audience :: Developers',
    'License :: OSI Approved :: Apache Software License',
    'Operating System :: OS Independent',
    'Programming Language :: Python',
    'Programming Language :: Python :: 3',
    'Topic :: Internet :: WWW/HTTP',
    'Topic :: Software Development :: Libraries',
    'Topic :: Software Development :: Quality Assurance',
    'Topic :: Utilities']

__copyright__ = '2013, %s ' % __author__
__license__ = """
    Copyright %s.

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either expressed or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
    """ % __copyright__


def run():
    """Run cpp coverage."""
    import os
    import sys
    from . import coverage, report

    args = coverage.create_args(sys.argv[1:])

    try:
        import yaml
    except ImportError:
        yaml = None

    yml = {}
    try:
        with open(args.coveralls_yaml, 'r') as fp:
            if not yaml:
                raise SystemExit(
                    'PyYAML is required for parsing configuration')
            yml = yaml.load(fp)
    except IOError:
        pass
    yml = yml or {}

    if not args.repo_token:
        args.repo_token = yml.get('repo_token', '')
    args.service_name = yml.get('service_name', 'travis-ci')
    args.service_job_id = os.environ.get('TRAVIS_JOB_ID', '')

    if not args.no_gcov:
        coverage.run_gcov(args)
    cov_report = coverage.collect(args)
    if args.verbose:
        print(cov_report)
    return report.post_report(cov_report)
