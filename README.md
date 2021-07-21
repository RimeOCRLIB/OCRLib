# OCRLib
OCR and linguistic AI library

OCRLib it is C++ library based on glyph features convolution network. Main features comparison based on 32x32 binary masks.
This masks contain model letters features and compared with recognised text.
Library also contain low level key-value database, fussy search class and image preprocessing.
Training process based on training set of letters or syllables images. 
For most languages training not need operator supervision. In development process training process improvement.

Core OCR based on 5-level convolutional network, associative search in low-level database and grammar analysis.
In development dynamic classification and deep-lerning algorithms for hand write text recognition on base of skeletisation.

More detailed documentation:
https://www.buddism.ru///ocrlib/documentation/OCRLib_documentation2018_eng.pdf
https://www.buddism.ru///ocrlib/Documentation_doxygen/

At present it is OCR more then million pages for www.tbrc.org and www.dharmabook.ru libraries with support of Trace Foundation www.trace.org, St.Petersburg State University and Moscow Dharma Center Rime community members.


The key features of the OCR system include:

High accuracy For Tibetan books, the current recognition results are 1-3 error per 1000 characters. It is include dictionary and mixed text. On next stages of development it is need same error level on manuscripts and damaged text OCR

Fast low-end database with associative search on base of Markov-chain algorithm 1gb/sec search with fuzzy query

Tibetan spell checker and dictionary on 359 000 words and 8.5 mln phrase list.

Poor resolution and damaged texts recognition. At present system OCR 72 dpi scanned texts. In development algorithm for hand write and xylographic text.

Python and HTML based user interface.

At present system trained for Tibetan, Sanskrit, Sinhala, Kannada, Latin and Cyrilic OCR.


HOW USE:
At present we have XCode version only. For build it is need fill config.h. 
In development progress CMake build for Linux, Windows and ARM Android.
If you have request or questions how to build OCRLib for your platform feel free to e-mail:gomde@mail.ru

Free OCR service for OCR on Tibetan, Sanskrit, Kannada and Sinhala www.dharmabook.ru/ocr/


With Best regards
Open Source Buddhism Library 
www.buddism.ru/ocrlib
YagpoOCR team gomde@mail.ru
