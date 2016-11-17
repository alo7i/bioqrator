--------------------------------------------------------------------------------
********************************************************************************
--------------------------------------------------------------------------------
|*|  PUBLIC DOMAIN NOTICE							 
|*|										 
|*| This work is a "United States Government Work" under the terms of the    
|*| United States Copyright Act. It was written as part of the authors'       
|*| official duties as a United States Government employee and thus cannot    
|*| be copyrighted within the United States. The data is freely available to 
|*| the public for use. The National Library of Medicine and the U.S.       
|*| Government have not placed any  restriction on its use or reproduction   
|*|           								        
|*| Although all reasonable efforts have been taken to ensure the accuracy and   
|*| reliability of the data and its source code, the NLM and the U.S. Government 
|*| do not and cannot warrant the performance or results that may be obtained by 
|*| using it. The NLM and the U.S. Government disclaim all warranties, express   
|*| or implied, including warranties of performance, merchantability or fitness  
|*| for any particular purpose. 						 
|*|										 
|*| Please cite the authors in any work or product based on this material:	
|*|
--------------------------------------------------------------------------------
********************************************************************************
--------------------------------------------------------------------------------

|*| BioC: A Minimalist Approach to Interoperability for Biomedical Text 
|*| Processing
|*| Donald C. Comeau, Rezarta Islamaj Dogan, Paolo Ciccarese, 
|*| Kevin Bretonnel Cohen, Martin Krallinger, Florian Leitner, Zhiyong Lu, 
|*| Yifan Peng, Fabio Rinaldi, Manabu Torii, Alfonso Valencia, Karin Verspoor, 
|*| Thomas C. Wiegers, Cathy H. Wu, and W. John Wilbur
|*| DATABASE, 2013




-------------------------
COMPILE and RUN
-------------------------

To build the programs issue the 'make' command in this 
directory. 
'make test' will run the programs and compare them with
their expected output.

-------------------------
WHAT is WHAT
-------------------------

This directory is organized as follows: 


	*. Directories: 
	(scroll down for specific details for each directory)

		1. BioC
 			-- the main BioC library files
		2. BioC-APPL-ABBR 
			-- BioC compliant application: abbreviation identifier
		3. BioC-APPL-SENT
			-- BioC compliant application: sentence identifier
		4. BioC-APPL-TOKEN
			-- BioC compliant application: tokenizer
		5. BioC-CORPUS-BioNLP09
			-- BioC compliant corpus: how to convert BioNLP09 to 
			BioC format
		6. BioC-SAMPLE
			-- Sample programs illustrating how to read/write a
			BioC XML data file
		7. iret
			-- Additional C++ classes used in the BioC library files
 
	*. Files:

		1. BioC.dtd
			-- description of BioC interchange format: the basic 
			collection, text in passages, text split into sentences, 
			annotations on the text in the sentences and in a passage, 
			relations and annotations on the text in the sentences, 
			and in a passage etc...
	
		2. Makefile
			-- Compiles everything by issueing 'make' in the top 
			directory.
			'make test' runs the programs and compares them with
			their expected output.
			'make clean' prepares for a new compilation removing
			all executables

		3. ReadMe - this file
	


-------------------------
BioC
-------------------------


This directory contains the core BIOC files, library, utility programs:
	
	a. BioC DTD -- see above

	b. BioC.cpp and BioC.hpp 
		the main BioC data classes

	c. BioC_libxml.cpp and BioC_libxml.hpp
		read / write BioC data using libxml parser

	d. BioC_util.cpp and BioC_util.hpp
		the utility class that automates walking the tree of BioC data
  		classes. Useful and available, but optional and 
		NOT part of the BioC proposal.



-------------------------
BioC-SAMPLE
-------------------------


This directory contains sample programs using the BioC library classes to 
illustrate data read/write from a BioC XML input.
 

	1. print_collection any.xml
  		Reads the data from any BioC XML file and prints it out in a
 		somewhat readable format. Demonstrates getting information in to
		and out of the BioC C++ classes.


	2. print_collection_reader any.xml
  		Same as print_collection, but uses the serial IO XML interface.


	3. ascii_collection collection.xml > out.xml
  		Reads the original collection including Unicode characters and
  		writes the same collection back out again using only ASCII
  		characters. out.xml should be the same as ascii.xml
  		ascii-data directory contain important files for the ascii
  		conversion program. The path of this directory is specified
		by path_strset file

	4. collection.xml, and ascii.xml are sample XML files to run sample 
		programs and illustrate the BioC code

	5. key files (collection.key and ascii.key) contain the description of
		contents of XML files
 
	6. collection.out 
 		contains the expected output when printing XML content in
		"readable" form. See Makefile for test commands that compare the
		sample programs' ouput to this file.  
	


------------------------------------------------------
BioC-Compliant APPLICATION: WORD AND SENTENCE TOKENIZER 
------------------------------------------------------


sentence.key
sentence_annotation.key
tokens.key



sentence.xml
sentence_annotation.xml
tokens.xml



sentence.cpp
sentence_annotation.cpp
sentence_serial.cpp


tokens.cpp





sentence ascii.xml > out.xml
	Takes a collection with the documents split into passages and splits
	each passage into a series of one or more sentences. out.xml should
	be the same as sentence.xml



sentence_serial ascii.xml > out.xml
	Same as sentence, but uses serial XML IO to both read and write the
	xml. Again, out.xml should be the same as sentence.xml



tokens sentence.xml > out.xml  
	Each sentence is split into individual tokens. These tokens are  
	represented as annotations on the original sentence. A simple
	example of an "annotation" as it would be unlikely to represent
	tokens in this manner. out.xml should match tokens.xml



sentence_annotation ascii.xml > out.xml  
	Sentences are represented as annotations on the original
	passages. Again a simple, but unlikely example. out.xml should match
	sentence_annotation.xml




---------------------------------------------------------------
BioC-Compliant APPLICATION: Ab3P (abbreviation identification)
---------------------------------------------------------------



Ab3P is a near state-of-the-art biomedical abbreviation short form
long form pair identifier. An independent installation can be
downloaded from http://www.ncbi.nlm.nih.gov/research/staff/wilbur/.


Sunghwan Sohn, Donald Comeau, Won Kim and W J Wilbur, Abbreviation
Definition Identification Based On Automatic Precision Estimates, BMC
Bioinformatics, 2008, 9:402.



abbr.key  
	Describes the BioC abbreviation XML file



abbr.cpp  
	abbr ascii.xml > abbr.html
	Identifies short form and long form pairs and records them with  
	passage level annotations.  



abbr.xml  
	Expected output from abbr



abbr_html.cpp  
	abbr_html ascii.xml abbr.xml  
	Uses the BioC abbreviation annotations to create an html page with
	the sample PubMed references where the abbreviation long form and  
	short form pairs are high-lighted. 
	
	The code does not know anything about abbreviations, although the 
	terms to appear in variable names and other text strings. Does
  	assume the annotations are in offset order and do not overlap.



abbr.html  
	Expected output from abbr_html



abbr.css  
	Used to display abbr.html



path_Ab3P  
	Used by abbr to find external data.



WordData  
	Binary data used by abbr. 
	Source data and programs to create these  
	files are available in the Ab3P distribution.




===================================
BioC-Compliant CORPUS" BioNLP'09
===================================
The BioNLP'09 Shared Task
(http://www.nactem.ac.uk/tsujii/GENIA/SharedTask/) dealt with
protein-protein interactions. We wanted to demonstrate that these
annotations and relations could be easily expressed via BioC.



bionlp.key  
	Describes the BioC XML files



bionlp_sample  
	Shared task sample files. The training, development, and test sets  
	of all been tested with BioC. They can be downloaded from the Shared  
	Task web site.



bionlp_copy  
	Temporary directory to hold a copy of the BioNLP annotations after  
	extracting from the BioC XML files. They can then be compared with  
	the original files.



bionlp_corpus.cpp  
	Create BioC corpus from bionlp .txt files


bionlp_annotate.cpp  
	Create BioC annotations from bionlp .a1 files


bionlp_relations.cpp  
	Create BioC annotations and relations from bionlp .a2 files


bionlp_corpus_dump.cpp  
	Recreate bionlp .txt files from BioC corpus


bionlp_annotate_dump.cpp  
	Recreate bionlp .a1 files from BioC annotations


bionlp_relations_dump.cpp  
	Recreate bionlp .a2 files from BioC annotations and relations



bionlp_test.sh  
	Run the above programs to create the BioC XML files, recreate the 
	bionlp shared task files, and verify that the content has been preserved.



------------------------
CONTACT
------------------------

Don Comeau:             comeau@ncbi.nlm.nih.gov
Rezarta Islamaj Dogan   Rezarta.Islamaj@nih.gov
John Wilbur:            wilbur@ncbi.nlm.nih.gov



------------------------
MORE INFORMATION: 
------------------------
A BioC webpage is available with all up-to-date instructions, code, and
corpora in the BioC format, and other research on, based on and related to
BioC. 


----------------------------------------------------------
LOCATION - OF - WEBPAGE:  
-----------------------------------------------------------
http://www.ncbi.nlm.nih.gov/CBBresearch/Dogan/BioC/


