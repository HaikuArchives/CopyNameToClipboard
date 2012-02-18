/*
 * Copyright 2000-2012. All rights reserved.
 * Distributed under the terms of the MIT license.
 *
 * Authors:
 *	Werner Freytag, freytag@gmx.de
 *	Humdinger, humdingerb@gmail.com
 */
 
#include <stdio.h>

#include <Clipboard.h>
#include <Entry.h>
#include <InterfaceDefs.h>
#include <Path.h>
#include <String.h>


extern "C" void
process_refs(entry_ref directoryRef, BMessage *msg, void *)
{
	BString ClipString;
	BPath path;

	int refs;
	entry_ref file_ref;
	
	bool filenameOnly = (modifiers() & B_SHIFT_KEY);
	bool onePerLine = (modifiers() & B_CONTROL_KEY);

	for (refs = 0; msg->FindRef("refs", refs, &file_ref) == B_NO_ERROR; refs++) {
		BEntry(&file_ref).GetPath(&path);

		if (refs>0)	{	// Delimiter for several file names
			if (onePerLine)
				ClipString.Append("\n");
			else
				ClipString.Append(" ");
		}
		
		BString	FileString = filenameOnly ? path.Leaf() : path.Path();
		bool quote = (FileString.FindFirst(" ") != B_ERROR);

		if (quote)
			ClipString.Append("\"").Append(FileString).Append("\"");
		else
			ClipString.Append(FileString);
	}

	if (refs != 0) {
		BClipboard Clipboard("system");
		BMessage *clip = (BMessage *)NULL;
		
		if (Clipboard.Lock()) {
			Clipboard.Clear();

			if ((clip = Clipboard.Data())) {
				clip->AddData("text/plain", B_MIME_TYPE, ClipString.String(),
					ClipString.Length()); 
				Clipboard.Commit();
			}
			Clipboard.Unlock();
		}
	}
}


int
main(int /*argc*/, char **/*argv*/)
{
	fprintf(stderr, "This can only be used as a Tracker add-on.\n");
	return -1; 
}
