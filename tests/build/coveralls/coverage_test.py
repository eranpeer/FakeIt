#!/usr/bin/env python
#
# Copyright 2013 (c) Lei Xu <eddyxu@gmail.com>

from . import coverage


def test_exclude_pattern():
    """Test using regular expression for exclusion rules."""
    args = coverage.create_args('-E /abc.*\.txt'.split())
    assert coverage.is_excluded_path(args, '/abcd.txt')
    assert not coverage.is_excluded_path(args, '/cd.txt')
