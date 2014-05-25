from __future__ import absolute_import

import io
import os
import re


class GcovLine:

    line_num = None
    cov_num = None
    text = None

    def __init__(self, line):
        line_tokens = line.split(':')
        self.cov_num = line_tokens[0].strip()
        self.line_num = int(line_tokens[1].strip())
        self.text = line_tokens[2]

    def is_title_line(self):
        return self.line_num == 0

    def is_line_without_code(self):
        return self.cov_num == '-'

    def should_ignore_line(self):
        return self.text.lstrip().startswith('static') or self.text.strip() == '}' or re.match(r'.*//\s*LCOV_EXCL_LINE\s*', self.text)

    def line_not_visited(self):
        return self.cov_num == '#####'

    def is_parse_error_line(self):
        return self.cov_num == '====='

    def calc_coverage(self):
        if self.is_title_line():
            return -1;
        if self.is_line_without_code():
            return None
        elif self.line_not_visited():
            if self.should_ignore_line():
                return None
            else:
                return 0
        elif self.is_parse_error_line():
            return 0;
        else:
            return int(self.cov_num)

class Context:

    CPP_EXTENSIONS = ['.h', '.hpp', '.cpp', '.cc', '.c']

    root = None
    build_root = None
    exclude_list = {}
    exclude_patterns = {}
    src_encoding = None

    def __init__(self, root, build_root, exclude_list, exclude_patterns, encoding):
        self.root = root
        self.build_root = build_root
        self.exclude_list = exclude_list
        self.exclude_patterns = exclude_patterns
        self.src_encoding = encoding

    def abs_root(self):
        return os.path.abspath(self.root)

    def abs_build_root(self):
        os.path.abspath(self.build_root)

    def path_from_root(self, abs_src_path):
        return os.path.relpath(abs_src_path, self.abs_root())

    def to_abs_src_path(self, src_path):
        abs_src_path = src_path
        if not os.path.isabs(src_path):
            abs_src_path = os.path.abspath(
                os.path.join(self.build_root, src_path))
        return abs_src_path

    def is_source_file(self, file_path):
        return os.path.splitext(file_path)[1] in self.CPP_EXTENSIONS

    def is_under_folder(self, abs_path, folder_abs_path):
        relative_to_folder_path = os.path.relpath(abs_path, folder_abs_path)
        if (relative_to_folder_path == '.'):
            # exact same path
            return True
        if len(relative_to_folder_path) > 3 and relative_to_folder_path[:3] != '../':
            return True
        return False

    def is_excluded_by_path(self, rel_file_path):
        abs_excluded_path = os.path.abspath(os.path.join(self.root,rel_file_path))
        for excluded_path in self.exclude_list:
            curr_abs_path = os.path.abspath(os.path.join(self.root,excluded_path))
            if os.path.isdir(curr_abs_path):
                return self.is_under_folder(abs_excluded_path, curr_abs_path)
            if curr_abs_path == abs_excluded_path:
                return True
        return False

    def is_excluded_by_regexp(self, file_path):
        for regexp_exclude_path in self.exclude_patterns:
            if re.match(regexp_exclude_path, file_path):
                return True
        return False;


    def is_excluded_path(self, relative_file_path):
        if self.is_excluded_by_regexp(relative_file_path):
            return True;
        return self.is_excluded_by_path(relative_file_path)

    def is_gcov_file(self, file_path):
        return os.path.splitext(file_path)[1] == '.gcov'


    def path_is_under_root(self, rel_src_path):
        return not (len(rel_src_path) > 3 and rel_src_path[:3] == '../')

    def read_src_file(self, abs_src_path):
        with io.open(abs_src_path, encoding=self.src_encoding) as src_file:
            file_read = src_file.read()
        return file_read

    def is_skip_dir(self, dir_name):
        skip_dirs = set(['.git', '.svn', '.deps'])
        return dir_name in skip_dirs

    def make_no_coverage_report(self, src_file_obj):
        coverage = []
        for line in src_file_obj:
            coverage.append(None)
        return coverage

    def make_empty_src_report(self, rel_src_path, abs_file_path):
        src_report = {}
        src_report['name'] = rel_src_path
        with io.open(abs_file_path, encoding=self.src_encoding) as src_file_obj:
            src_report['coverage'] = self.make_no_coverage_report(src_file_obj)
            src_file_obj.seek(0)
            src_report['source'] = src_file_obj.read()
        return src_report

    def parse_coverage(self, file_obj):
        coverage = []
        for line in file_obj:
            cov_line = GcovLine(line)
            if not cov_line.is_title_line():
                coverage.append(cov_line.calc_coverage())
        return coverage

    def make_src_report(self, file_obj):
        source_file_line = file_obj.readline()
        src_path = source_file_line.split(':')[-1].strip()
        abs_src_path = self.to_abs_src_path(src_path)
        rel_src_path = self.path_from_root(abs_src_path)
        src_report = None;
        if self.path_is_under_root(rel_src_path) and not self.is_excluded_path(rel_src_path):
            src_report = {
                'name': rel_src_path,
                'source': self.read_src_file(abs_src_path),
                'coverage': self.parse_coverage(file_obj)
            }
        return src_report


    def make_source_reports(self):
        source_reports = []
        discovered_files = set()
        for curr_dir, dir_names, file_names in os.walk(self.root):
            if self.is_skip_dir(os.path.basename(curr_dir)):
                dir_names[:] = []
                continue

            rel_dir_path = self.path_from_root(os.path.abspath(curr_dir));
            if self.is_excluded_path(rel_dir_path):
                dir_names[:] = []
                continue

            for file_name in file_names:
                if self.is_gcov_file(file_name):
                    abs_file = os.path.abspath(os.path.join(curr_dir, file_name))
                    with open(abs_file) as src_file_obj:
                        src_report = self.make_src_report(src_file_obj)
                        if src_report is not None:
                            discovered_files.add(src_report['name'])
                            source_reports.append(src_report)

        # Also collects the source file_names that have no coverage reports.
        for curr_dir, dir_names, file_names in os.walk(self.root):
            if self.is_skip_dir(os.path.basename(curr_dir)):
                dir_names[:] = []
                continue

            rel_dir_path = self.path_from_root(os.path.abspath(curr_dir));
            if (self.is_excluded_path(rel_dir_path)):
                dir_names[:] = []
                continue

            for file_name in file_names:
                if not self.is_source_file(file_name):
                    continue
                abs_file_path = os.path.abspath(os.path.join(curr_dir, file_name))
                rel_src_path = self.path_from_root(abs_file_path)
                if self.is_excluded_path(rel_src_path):
                    continue
                if not rel_src_path in discovered_files:
                    src_report = self.make_empty_src_report(rel_src_path, abs_file_path)
                    source_reports.append(src_report)
        return source_reports
