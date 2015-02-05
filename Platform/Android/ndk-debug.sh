#!/bin/bash
#
#  ndk-debug.sh
#  ePub3
#
#  Created by Pedro Reis Colaco (txtr) on 2013-05-31.
#  
#  Copyright (c) 2014 Readium Foundation and/or its licensees. All rights reserved.
#  
#  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
#  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
#  
#  Licensed under Gnu Affero General Public License Version 3 (provided, notwithstanding this 
#  notice, Readium Foundation reserves the right to license this material under a different 
#  separate license, and if you have done so, the terms of that separate license control and 
#  the following references to GPL do not apply).
#  
#  This program is free software: you can redistribute it and/or modify it under the terms 
#  of the GNU Affero General Public License as published by the Free Software Foundation, 
#  either version 3 of the License, or (at your option) any later version. You should have 
#  received a copy of the GNU Affero General Public License along with this program.  If not, 
#  see <http://www.gnu.org/licenses/>.


NDK_PATH="$ANDROID_NDK"
[[ ( "$1" != "--help" || "$NDK_PATH" == "" ) && "$1" != "" ]] && {
    NDK_PATH="$1"
}

[[ "$1" == "--help" || "$NDK_PATH" == "" ]] && {
	echo "usage: ndk-debug.sh [--help] [<path-to-ndk>]"
	echo "       --help         Shows this help message."
	echo "       <path-to-ndk>  The path to the Android NDK to use."
	echo ""
	echo "       The path to NDK can be set by a global environment variable ANDROID_NDK,"
	echo "       to avoid being passed by argument to this script. "
	exit 1
}

PROJECT_PATH=`pwd`

echo "Starting ndk-gdb..."
echo ""
echo "Project path: $PROJECT_PATH"
echo "NDK path: $NDK_PATH"
echo ""

$NDK_PATH/ndk-gdb --project="$PROJECT_PATH"
