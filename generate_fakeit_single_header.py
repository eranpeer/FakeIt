#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import io
import os
import sys
import re
import datetime

fakeitPath = "."

includesMatcher = re.compile( r'\s*#include\s*"(.*)"' )
pragmaOnceMatcher = re.compile( r'\s*#pragma\s*once\s*' )

processedHeaders = set([])

def write_line(out, line ):
    out.write( line + "\n")

def parseFile(out, config, filename):
    print( "parsing " + filename )

    if os.path.exists( os.path.join( fakeitPath, 'config' , config, filename) ):
        filePath = os.path.join( fakeitPath, 'config' , config, filename)        
    elif  os.path.exists( os.path.join( fakeitPath, 'include' , filename) ):
        filePath = os.path.join( fakeitPath, 'include' , filename)
    else:        
        return        

    write_line(out, "// #included from: " + filename )

    f = open(filePath, 'r' )
    for line in f:
        m = includesMatcher.match( line )
        if m:
            header = m.group(1)
            if not header in processedHeaders:
                processedHeaders.add( header )
                parseFile(out, config, header)
        else:
            write_line(out, line.rstrip() )


def writeHeaderComment(out, config):
    write_line(out, "/*" )
    write_line(out, " *  FakeIt - A Simplified C++ Mocking Framework" )
    write_line(out, " *  Copyright (c) Eran Pe'er 2013" )
    write_line(out, " *  Generated: {0}".format( datetime.datetime.now()) )
    write_line(out, " *  Distributed under the MIT License. Please refer to the LICENSE file at:")
    write_line(out, " *  https://github.com/eranpeer/FakeIt" )
    write_line(out, " */" )


def generateFakeitIncludeFile(out, config):
    parseFile(out, config, 'fakeit.hpp' )

comment_re = re.compile(
    r'(^)?[^\S\n]*/(?:\*(.*?)\*/[^\S\n]*|/[^\n]*)($)?',
    re.DOTALL | re.MULTILINE
)

def comment_replacer(match):
    start,mid,end = match.group(1,2,3)
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
    return comment_re.sub(comment_replacer, text)

def remove_pragma_once(text):
    return pragmaOnceMatcher.sub('\n', text)

def createFakeitFileText(config):
    textStream  = io.StringIO()
    generateFakeitIncludeFile(textStream, config)
    text = textStream.getvalue()
    text = remove_comments(text)
    text = remove_pragma_once(text)
    return text

def createFakeitFile(config,text):
    dest_dir = os.path.join( fakeitPath, 'single_header', config)
    if (not os.path.exists(dest_dir)):
    	os.makedirs(dest_dir)
    outputPath = os.path.join( dest_dir, 'fakeit.hpp' )
    out = open( outputPath, 'w' )
    write_line(out, "#pragma once" )
    writeHeaderComment(out, config)
    out.write(text)
    out.close()


config = sys.argv[1]
text = createFakeitFileText(config)
createFakeitFile(config,text)
