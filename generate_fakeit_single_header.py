#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import argparse
import datetime
import io
import os
import re

fakeit_path = os.path.dirname(__file__)

includes_matcher = re.compile(r'\s*#include\s*"(.*)"')
pragma_once_matcher = re.compile(r'\s*#pragma\s*once\s*')
comment_matcher = re.compile(
    r'(^)?[^\S\n]*/(?:\*(.*?)\*/[^\S\n]*|/[^\n]*)($)?',
    re.DOTALL | re.MULTILINE
)

processed_headers = set([])


def write_line(out, line):
    out.write(line + "\n")


def parse_file(out, config, filename):
    print("parsing " + filename)

    if os.path.exists(os.path.join(fakeit_path, 'config', config, filename)):
        file_path = os.path.join(fakeit_path, 'config', config, filename)
    elif os.path.exists(os.path.join(fakeit_path, 'include', filename)):
        file_path = os.path.join(fakeit_path, 'include', filename)
    else:
        return

    write_line(out, "// #included from: " + filename)

    f = open(file_path, 'r')
    for line in f:
        m = includes_matcher.match(line)
        if m:
            header = m.group(1)
            if header not in processed_headers:
                processed_headers.add(header)
                parse_file(out, config, header)
        else:
            write_line(out, line.rstrip())


def write_header_comment(out, config):
    write_line(out, "/*")
    write_line(out, " *  FakeIt - A Simplified C++ Mocking Framework")
    write_line(out, " *  Copyright (c) Eran Pe'er 2013")
    write_line(out, " *  Generated: {0}".format(datetime.datetime.now()))
    write_line(out, " *  Distributed under the MIT License. Please refer to the LICENSE file at:")
    write_line(out, " *  https://github.com/eranpeer/FakeIt")
    write_line(out, " */")


def generate_fakeit_include_file(out, config):
    parse_file(out, config, 'fakeit.hpp')


def comment_replacer(match):
    start, mid, end = match.group(1, 2, 3)
    if mid is None:
        # single line comment
        return ''
    elif start is not None or end is not None:
        # multi line comment at start or end of a line
        return ''
    elif '\n' in mid:
        # multi line comment with line break
        return '\n'
    else:
        # multi line comment without line break
        return ' '


def remove_comments(text):
    return comment_matcher.sub(comment_replacer, text)


def remove_pragma_once(text):
    return pragma_once_matcher.sub('\n', text)


def create_fakeit_file_text(config):
    text_stream = io.StringIO()
    generate_fakeit_include_file(text_stream, config)
    text = text_stream.getvalue()
    text = remove_comments(text)
    text = remove_pragma_once(text)
    return text


def create_fakeit_file(config, text):
    dest_dir = os.path.join(fakeit_path, 'single_header', config)
    if not os.path.exists(dest_dir):
        os.makedirs(dest_dir)
    output_path = os.path.join(dest_dir, 'fakeit.hpp')
    out = open(output_path, 'w')
    write_line(out, "#pragma once")
    write_header_comment(out, config)
    out.write(text)
    out.close()


def create_fakeit_file_for_config(config):
    processed_headers.clear()
    text = create_fakeit_file_text(config)
    create_fakeit_file(config, text)


def get_all_configs():
    return os.listdir(os.path.join(fakeit_path, 'config'))


def get_parser():
    parser = argparse.ArgumentParser()
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument('-c', '--configs', nargs='+', metavar='config', choices=get_all_configs())
    group.add_argument('-a', '--all', action='store_true')
    return parser


def main():
    args = get_parser().parse_args()

    if args.all:
        configs = get_all_configs()
    else:
        configs = args.configs

    for config in configs:
        print(f"writing single header file for {config}")
        create_fakeit_file_for_config(config)


if __name__ == '__main__':
    main()
