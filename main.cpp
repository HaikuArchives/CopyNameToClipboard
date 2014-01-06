#include <Clipboard.h>
#include <Entry.h>
#include <Path.h>
#include <InterfaceDefs.h>
#include <String.h>
#include <TrackerAddon.h>

void process_refs(entry_ref dir_ref, BMessage *msg, void *) {

	BString		ClipString;
	BPath		path;

	int			refs;
	entry_ref	file_ref;
	
	bool		filenameonly = (modifiers() & B_SHIFT_KEY);

	for ( refs = 0; msg->FindRef("refs", refs, &file_ref) == B_NO_ERROR; refs++ ) { 
		BEntry(&file_ref).GetPath(&path);

		if (refs>0) ClipString.Append(" ");

		BString	FileString	= filenameonly ? path.Leaf() : path.Path();
		bool	quote = (FileString.FindFirst(" ") != B_ERROR );

		if (quote) ClipString.Append("\"").Append(FileString).Append("\"");
		else ClipString.Append(FileString);
	}

	if (refs != 0) {
		BClipboard	Clipboard("system");
		BMessage	*clip = (BMessage *)NULL;
		
		if (Clipboard.Lock()) {

			Clipboard.Clear();
			if ( (clip = Clipboard.Data()) ) {
				clip->AddData("text/plain", B_MIME_TYPE, ClipString.String(), ClipString.Length()); 
				Clipboard.Commit();
			}
			Clipboard.Unlock();
		}
	}
}
