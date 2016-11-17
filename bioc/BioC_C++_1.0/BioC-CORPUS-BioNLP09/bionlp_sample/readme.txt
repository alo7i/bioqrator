Sample Data for BioNLP'09 Shared Task on Event Extraction

- Contains 6 PubMed abstracts and their named entity and molecular event
  annotations.

- For format details, please refer to the shared task homepage.
  http://www-tsujii.is.s.u-tokyo.ac.jp/GENIA/SharedTask/

- This data is open to the public and can be freely distributed,
  but please preserve the package contents as is to avoid causing
  any confusion.


File Descriptions:

*.txt	files contain the target text.
	Each of these files consists of two lines, one for the title and one for the abstract.
*.a1	files contain standoff annotations for proteins.
	The participants will be provided with these files.
*.a2	files contain standoff annotations for other named entities and events.
	For training, the participants will be provided with these files.
	For test, the participants have to create this annotation.


History:

19 January 2009, Revision 3 released.
	- Some annotations for speculation and negation are corrected.

22 December 2008, Revision 2 released.
	- ToLoc slots have been added to Localization events.
	- Duplicate text-bound annotations have been removed.
	- The annotations for equivalent terms have been moved from *.a1 to *.a2.

16 December 2008, Revision 1 released.
	- The text span of entities and trigger expressions has been added
	  to the "T" type annotation for better readability.
	- A couple of annotations have been revised.

15 December 2008, The first version released.


Authors:

BioNLP'09 Shared Task organizers
bionlpsharedtask@gmail.com
