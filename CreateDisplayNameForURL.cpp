/*
 *  Copyright (C) 2010 Stephen F. Booth <me@sbooth.org>
 *  All Rights Reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 *
 *    - Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    - Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    - Neither the name of Stephen F. Booth nor the names of its 
 *      contributors may be used to endorse or promote products derived
 *      from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <Carbon/Carbon.h>

#include "AudioEngineDefines.h"
#include "CreateDisplayNameForURL.h"


CFStringRef
CreateDisplayNameForURL(CFURLRef url)
{
	assert(NULL != url);
	
	CFStringRef displayName = NULL;

	CFStringRef scheme = CFURLCopyScheme(url);
	bool isFileURL = (kCFCompareEqualTo == CFStringCompare(CFSTR("file"), scheme, kCFCompareCaseInsensitive));
	CFRelease(scheme), scheme = NULL;

	if(isFileURL) {
		FSRef ref;

		if(false == CFURLGetFSRef(url, &ref)) {
			ERR("Unable to get FSRef for URL");
			return NULL;
		}
		
		OSStatus result = LSCopyItemAttribute(&ref, 
											  kLSRolesAll, 
											  kLSItemDisplayName, 
											  reinterpret_cast<CFTypeRef *>(&displayName));
		
		if(noErr != result) {
			ERR("LSCopyItemAttribute (kLSItemDisplayName) failed: %i", result);
			return NULL;
		}
	}
	else {
		displayName = CFURLGetString(url);
		CFRetain(displayName);
	}

	return displayName;
}
