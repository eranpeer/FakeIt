import os
import sys
import re
import datetime

fakeitPath = "."

includesMatcher = re.compile( r'\s*#include\s*"(.*)"' )
defineMatcher = re.compile( r'\s*#define')
startMultilineCommentMatcher = re.compile( r'^\s*/\*')
multilineCommentMatcher = re.compile( r'^ \*')
pragmaOnceMatcher = re.compile( r'\s*#pragma\s*once\s*' )

#includePath = os.path.join( fakeitPath, 'include/' )

processedHeaders = set([])

def write(out, line ):
    out.write( line )

def parseFile(out, config, filename):
    print( "parsing " + filename )

    if os.path.exists( os.path.join( fakeitPath, 'config' , config, filename) ):
        filePath = os.path.join( fakeitPath, 'config' , config, filename)        
    elif  os.path.exists( os.path.join( fakeitPath, 'include' , filename) ):
        filePath = os.path.join( fakeitPath, 'include' , filename)
    else:        
        return        

    write(out, "// #included from: " + filename + "\n" )

    f = open(filePath, 'r' )
    for line in f:
        m = includesMatcher.match( line )
        if m:
            header = m.group(1)
            if not header in processedHeaders:
                processedHeaders.add( header )
                parseFile(out, config, header)
            elif not pragmaOnceMatcher.match( line ):             
                write(out, line.rstrip() + "\n" )

def writeHeaderComment(out, config):
    out.write( "/*\n" )
    out.write( " *  FakeIt - A Simplified C++ Mocking Framework\n" )
    out.write( " *  Copyright (c) Eran Pe'er 2013\n" )
    out.write( " *  Generated: {0}\n".format( datetime.datetime.now()) )
    out.write( " *  Distributed under the MIT License. Please refer to the LICENSE file at:\n")
    out.write( " *  https://github.com/eranpeer/FakeIt\n" )
    out.write( " */\n" )


def generateFakeitIncludeFile(out, config):
    writeHeaderComment(out, config)
    out.write( "#pragma once\n" )
    parseFile(out, config, 'fakeit.hpp' )


config = sys.argv[1]
os.makedirs(os.path.join( fakeitPath, 'single_include', config))
outputPath = os.path.join( fakeitPath, 'single_include', config, 'fakeit.hpp' )
out = open( outputPath, 'w' )

generateFakeitIncludeFile(out, config)