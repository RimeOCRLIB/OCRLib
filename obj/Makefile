#CXX=arm-linux-androideabi-g++
#CXX=aarch64-linux-android-g++
#CXX=clang++
CXX=cc
#LIB=libre2.so.0
BUILD=3

OBJ=main.o GBitmap.o GBitmap32.o GBitmapAutoLevel.o GBitmapBinarisation.o GBitmapDetectorRadon.o GBitmapFilter.o GBitmapFocalLine.o GBitmapPenWidth.o GBitmapRotate.o GBitmapScale.o GBitmapTextOrientation.o lodepng.o  OCRAlgorithm.o  MmapFile.o GVector.o  GMap.o associativeSignalProcessing.o lookupProcess3.o lookupProcess4.o lookupProcessFontMatch3.o tableDict.o tableLetters.o tableProcessing.o tableProcessingDict.o tableProcessingFontMatch.o tableProcessingFullMatch.o tableProcessingID.o tableProcessingOCRMatch.o tableProcessingStack.o tableText.o  GMemory.o loadDB.o  php2stl.o pugixml.o pugixpath.o GGraph.o GGraphAlgorithm.o  GGraphAttributeLinePixels.o GGraphBuildFocalLineTable.o  GGraphDetectFocalPoint.o GGraphFindExtremPoints.o GGraphFocalLine.o GGraphFocalLineRecognition.o GGraphFocalPoint.o GGraphStableFocalPoint.o GGraphLetter.o GGraphLetterCorrelation.o GGraphOCRStar.o OCRFont.o GGraphSSE.o GGraphOCR.o GGraphOCRBuildOCRTable.o GGraphOCRCorrelation.o GGraphOCRFont.o GGraphOCRBackwardCorrelation.o GGraphOCRStarCorrelation.o GGraphOCRCorrelationlookup2.o GGraphOCRCorrelationlookup3.o GGraphOCRCorrelationlookup4.o GGraphOCRTextCorrelation.o GGraphOCRTextCorrelationHash.o GGraphOCRTextOCR.o GDraw.o GDraw_Graph.o OCRTypes.o cpuTest.o  GGraphBezier.o GGraphBuildLetterSaBezier.o GGraphCorrelationBezierFullBezier.o GGraphCorrelationBezierPartBezier.o GGraphFocalLineBezier.o GGraphFocalLineRecLevelingBezier.o GGraphFocalLineRecognitionBezier.o GGraphFocalPointBezier.o GGraphLineMatrixBezier.o GGraphStableFocalPointBezier.o GGraphTestBezier.o GImageEditor.o GImageEditor_functions.o GImageEditorAdaptiveFiltration.o OCRLib_GGraphOCR.o imageProcessOCR.o

OBJ_MAIN=GBitmask.o GBitset.o GBitsetCorrectSqH.o GBitsetCorrectSqL.o GBitsetMatrix_base.o GBitsetMatrix_level.o GBitsetNet.o GBitsetOCR.o GBitsetPageSegmentation.o GBitsetPoint.o GBitsetStructure.o GBitsetTest.o GBitsetWord.o GBitsetWordEng.o GBitsetWordTib.o GBitset_Letter.o GBitset_Starting.o GBitset_algorithm.o GBitset_base.o GBitset_baseOCR.o GBitset_level.o GBitset_setDataOCR.o GContour.o GFont.o GFontEditor.o GLetter.o GLogicProcessor.o GMainEditor.o GMemoryRequest.o LoadFontMaps.o MartixCorrelation.o Matrix_Lion.o OCRLib_Editor.o OCRLib_Image.o OCRLib_OCR.o UniToYagpo.o algorithmOCR_128.o algorithmOCR_32.o buidSearchString.o buidSearchStringNew.o buildLetter.o buildOpenType.o classification.o codePagesConverter.o collectStackLetter.o collectTextLine.o compressMatch.o converterPali.o dictReport.o dictReportLocal.o drawBase.o drawGrapeLine.o drawMatchLine.o drawPage.o grammarCorrector.o grammarOCR.o intersectionArea.o letterAssociation.o letterConstructions.o letterCorrelation.o letterNeighbors.o letterNeighborsNew.o letterNeighborsSanskrit.o letterOCR.o letterProbability.o letterToLetterCorrelation.o lineOCR.o loadDictionary.o loadMap.o matrixT.o pageOCR.o pageSegmentation.o processInput.o readInputAction.o renderDictSearch.o renderDictSearchNew.o renderDictSearchSanskrit.o sentenceConstructor.o sentenceConstructorNew.o setImageOCR.o setImageParamOCR.o setLetter.o setLetterScale.o setMatrix_Point_32_64C.o setMatrix_Point_32_64_SSE.o setMatrix_Point_64ASM_ATT.o setMatrix_Point_64ASM_INTEL_F.o setMatrix_Point_64_popcnt_C.o setOCRCorrelation.o setOCRRecordCorrelation.o setScaleBitsetOCR.o setScaleOCR.o setScaleRecord.o spellChecker.o startConvert.o startDictionary.o startOCR.o vectorOCR.o testLetter.o testStack.o testWordLine.o textOCR.o  OCRLib.o

ifeq ($(BUILD),1)  #полный исполняемый файл
CXX=clang++
all:  ocrlib
ocrlib: $(OBJ) $(OBJ_MAIN)
	$(CXX) $(OBJ) $(OBJ_MAIN) -o ../ocrlib
endif

ifeq ($(BUILD),2) #сокращенная динамическая библиотека
CXX=clang++
all:  libocr
libocr: $(OBJ)
	$(CXX) $(OBJ) -shared -Wall -o libocr.so
	
	cp libocr.so /usr/lib/libocr.so
	cp libocr.so /OSBL/src/libocr.so
endif

ifeq ($(BUILD),3)  #полная динамическая библиотека
CXX=clang++
all:  libocr
libocr: $(OBJ) $(OBJ_MAIN)
	$(CXX) $(OBJ) $(OBJ_MAIN) -shared -Wall -o ../libocr.so
	
	#cp ../libocr.so /usr/lib/libocr.so
	cp ../libocr.so /OSBL/GUI/src/libocr.so
endif

ifeq ($(BUILD),4) #сокращенный модуль Unit
CXX=cc
OBJ=nxt_unit_app_test.o
all: nxt_unit_app_test
nxt_unit_app_test: $(OBJ)
	$(CXX) $(OBJ) -Wall -L. -locr  ../unit/build/libunit.a -o nxt_unit_app_test
endif

ifeq ($(BUILD),5)
CXX=cc
OBJ=test_lib.o
all:  test_lib
test_lib: $(OBJ)
	$(CXX) $(OBJ) -Wall -L. -locr -o test_lib
endif



nxt_unit_app_test.o:  ../unit/src/test/nxt_unit_app_test.c
	$(CXX) ../unit/src/test/nxt_unit_app_test.c -I "../unit/src" -I "../unit/build" -c -fPIC 

test_lib.o:  ../test_lib.c
	$(CXX) ../test_lib.c -c -fPIC	

	
main.o:  ../main.cpp
	$(CXX) ../main.cpp -c -fPIC	
GBitmap.o: ../libocr/GBitmap/GBitmap.cpp
	$(CXX) ../libocr/GBitmap/GBitmap.cpp -c -fPIC
GBitmap32.o: ../libocr/GBitmap/GBitmap32.cpp
	$(CXX) ../libocr/GBitmap/GBitmap32.cpp -c -fPIC
GBitmapAutoLevel.o: ../libocr/GBitmap/GBitmapAutoLevel.cpp 
	$(CXX) ../libocr/GBitmap/GBitmapAutoLevel.cpp -c -fPIC
GBitmapBinarisation.o: ../libocr/GBitmap/GBitmapBinarisation.cpp
	$(CXX) ../libocr/GBitmap/GBitmapBinarisation.cpp -c -fPIC
GBitmapDetectorRadon.o: ../libocr/GBitmap/GBitmapDetectorRadon.cpp
	$(CXX) ../libocr/GBitmap/GBitmapDetectorRadon.cpp -c -fPIC
GBitmapFilter.o: ../libocr/GBitmap/GBitmapFilter.cpp 
	$(CXX) ../libocr/GBitmap/GBitmapFilter.cpp -c -fPIC
GBitmapFocalLine.o: ../libocr/GBitmap/GBitmapFocalLine.cpp
	$(CXX) ../libocr/GBitmap/GBitmapFocalLine.cpp -c -fPIC
GBitmapPenWidth.o: ../libocr/GBitmap/GBitmapPenWidth.cpp 
	$(CXX) ../libocr/GBitmap/GBitmapPenWidth.cpp -c -fPIC
GBitmapRotate.o:  ../libocr/GBitmap/GBitmapRotate.cpp
	$(CXX) ../libocr/GBitmap/GBitmapRotate.cpp -c -fPIC
GBitmapScale.o: ../libocr/GBitmap/GBitmapScale.cpp
	$(CXX) ../libocr/GBitmap/GBitmapScale.cpp -c -fPIC
GBitmapTextOrientation.o: ../libocr/GBitmap/GBitmapTextOrientation.cpp
	$(CXX) ../libocr/GBitmap/GBitmapTextOrientation.cpp -c -fPIC
	
lodepng.o: ../libocr/PNG/lodepng.cpp
	$(CXX) ../libocr/PNG/lodepng.cpp -c -fPIC
OCRAlgorithm.o: ../libocr/OCRAlgorithm/OCRAlgorithm.cpp 
	$(CXX) ../libocr/OCRAlgorithm/OCRAlgorithm.cpp -c -fPIC

GMemory.o: ../libocr/GMemory/GMemory.cpp
	$(CXX) ../libocr/GMemory/GMemory.cpp -c -fPIC
loadDB.o: ../libocr/GMemory/loadDB.cpp
	$(CXX) ../libocr/GMemory/loadDB.cpp -c -fPIC
MmapFile.o: ../libocr/GMemory/MmapFile.cpp
	$(CXX) ../libocr/GMemory/MmapFile.cpp -c -fPIC
GVector.o: ../libocr/GMemory/GVector.cpp 
	$(CXX) ../libocr/GMemory/GVector.cpp -c -fPIC
GMap.o: ../libocr/GMemory/GMap/GMap.cpp
	$(CXX) ../libocr/GMemory/GMap/GMap.cpp -c -fPIC
associativeSignalProcessing.o: ../libocr/GMemory/GMap/associativeSignalProcessing.cpp
	$(CXX) ../libocr/GMemory/GMap/associativeSignalProcessing.cpp -c -fPIC
lookupProcess3.o: ../libocr/GMemory/GMap/lookupProcess3.cpp
	$(CXX) ../libocr/GMemory/GMap/lookupProcess3.cpp -c -fPIC
lookupProcess4.o: ../libocr/GMemory/GMap/lookupProcess4.cpp
	$(CXX) ../libocr/GMemory/GMap/lookupProcess4.cpp -c -fPIC
lookupProcessFontMatch3.o: ../libocr/GMemory/GMap/lookupProcessFontMatch3.cpp
	$(CXX) ../libocr/GMemory/GMap/lookupProcessFontMatch3.cpp -c -fPIC
tableDict.o: ../libocr/GMemory/GMap/tableDict.cpp
	$(CXX) ../libocr/GMemory/GMap/tableDict.cpp -c -fPIC
tableLetters.o: ../libocr/GMemory/GMap/tableLetters.cpp
	$(CXX) ../libocr/GMemory/GMap/tableLetters.cpp -c -fPIC
tableProcessing.o: ../libocr/GMemory/GMap/tableProcessing.cpp
	$(CXX) ../libocr/GMemory/GMap/tableProcessing.cpp -c -fPIC
tableProcessingDict.o: ../libocr/GMemory/GMap/tableProcessingDict.cpp
	$(CXX) ../libocr/GMemory/GMap/tableProcessingDict.cpp -c -fPIC
tableProcessingFontMatch.o: ../libocr/GMemory/GMap/tableProcessingFontMatch.cpp
	$(CXX) ../libocr/GMemory/GMap/tableProcessingFontMatch.cpp -c -fPIC
tableProcessingFullMatch.o: ../libocr/GMemory/GMap/tableProcessingFullMatch.cpp
	$(CXX) ../libocr/GMemory/GMap/tableProcessingFullMatch.cpp -c -fPIC
tableProcessingID.o: ../libocr/GMemory/GMap/tableProcessingID.cpp
	$(CXX) ../libocr/GMemory/GMap/tableProcessingID.cpp -c -fPIC
tableProcessingOCRMatch.o: ../libocr/GMemory/GMap/tableProcessingOCRMatch.cpp
	$(CXX) ../libocr/GMemory/GMap/tableProcessingOCRMatch.cpp -c -fPIC
tableProcessingStack.o: ../libocr/GMemory/GMap/tableProcessingStack.cpp
	$(CXX) ../libocr/GMemory/GMap/tableProcessingStack.cpp -c -fPIC
tableText.o: ../libocr/GMemory/GMap/tableText.cpp
	$(CXX) ../libocr/GMemory/GMap/tableText.cpp -c -fPIC
	
GMemoryRequest.o: ../libocr/GMemory/GMemoryRequest.cpp
	$(CXX) ../libocr/GMemory/GMemoryRequest.cpp -c

OCRTypes.o: ../libocr/OCRTypes/OCRTypes.cpp
	$(CXX) ../libocr/OCRTypes/OCRTypes.cpp -c -fPIC
cpuTest.o: ../libocr/OCRString/cpuTest.cpp
	$(CXX) ../libocr/OCRString/cpuTest.cpp -c -fPIC
 php2stl.o: ../libocr/OCRString/php2stl.cpp
	$(CXX) ../libocr/OCRString/php2stl.cpp -c -fPIC 
pugixml.o: ../pugixml/pugixml.cpp 
	$(CXX) ../pugixml/pugixml.cpp -c -fPIC
pugixpath.o:
	$(CXX) ../pugixml/pugixpath.cpp -c -fPIC
	
GGraph.o: ../libocr/GGraph/GGraph.cpp
	$(CXX) ../libocr/GGraph/GGraph.cpp -c -fPIC
GGraphAlgorithm.o: ../libocr/GGraph/GGraphAlgorithm.cpp
	$(CXX) ../libocr/GGraph/GGraphAlgorithm.cpp -c -fPIC
GGraphAttributeLinePixels.o: ../libocr/GGraph/GGraphAttributeLinePixels.cpp
	$(CXX) ../libocr/GGraph/GGraphAttributeLinePixels.cpp -c -fPIC
GGraphBuildFocalLineTable.o: ../libocr/GGraph/GGraphBuildFocalLineTable.cpp
	$(CXX) ../libocr/GGraph/GGraphBuildFocalLineTable.cpp -c -fPIC
GGraphDetectFocalPoint.o: ../libocr/GGraph/GGraphDetectFocalPoint.cpp
	$(CXX) ../libocr/GGraph/GGraphDetectFocalPoint.cpp -c -fPIC
GGraphFindExtremPoints.o: ../libocr/GGraph/GGraphFindExtremPoints.cpp
	$(CXX) ../libocr/GGraph/GGraphFindExtremPoints.cpp -c -fPIC
GGraphFocalLine.o: ../libocr/GGraph/GGraphFocalLine.cpp
	$(CXX) ../libocr/GGraph/GGraphFocalLine.cpp -c -fPIC
GGraphFocalLineRecognition.o: ../libocr/GGraph/GGraphFocalLineRecognition.cpp
	$(CXX) ../libocr/GGraph/GGraphFocalLineRecognition.cpp -c -fPIC
GGraphFocalPoint.o: ../libocr/GGraph/GGraphFocalPoint.cpp
	$(CXX) ../libocr/GGraph/GGraphFocalPoint.cpp -c -fPIC
GGraphLetter.o: ../libocr/GGraph/GGraphLetter.cpp
	$(CXX) ../libocr/GGraph/GGraphLetter.cpp -c -fPIC
GGraphLetterCorrelation.o: ../libocr/GGraph/GGraphLetterCorrelation.cpp
	$(CXX) ../libocr/GGraph/GGraphLetterCorrelation.cpp -c -fPIC
GGraphOCRStar.o: ../libocr/GGraph/GGraphOCRStar.cpp
	$(CXX) ../libocr/GGraph/GGraphOCRStar.cpp -c -fPIC
GGraphSSE.o: ../libocr/GGraph/GGraphSSE.cpp
	$(CXX) ../libocr/GGraph/GGraphSSE.cpp -c -fPIC
GGraphStableFocalPoint.o: ../libocr/GGraph/GGraphStableFocalPoint.cpp
	$(CXX) ../libocr/GGraph/GGraphStableFocalPoint.cpp -c -fPIC
OCRFont.o: ../libocr/GGraph/OCRFont.cpp
	$(CXX) ../libocr/GGraph/OCRFont.cpp -c -fPIC


GGraphOCR.o: ../libocr/GGraphOCR/GGraphOCR.cpp
	$(CXX) ../libocr/GGraphOCR/GGraphOCR.cpp -c -fPIC
GGraphOCRBuildOCRTable.o: ../libocr/GGraphOCR/GGraphOCRBuildOCRTable.cpp
	$(CXX) ../libocr/GGraphOCR/GGraphOCRBuildOCRTable.cpp -c -fPIC
GGraphOCRCorrelation.o: ../libocr/GGraphOCR/GGraphOCRCorrelation.cpp
	$(CXX) ../libocr/GGraphOCR/GGraphOCRCorrelation.cpp -c -fPIC
GGraphOCRFont.o: ../libocr/GGraphOCR/GGraphOCRFont.cpp
	$(CXX) ../libocr/GGraphOCR/GGraphOCRFont.cpp -c -fPIC
GGraphOCRBackwardCorrelation.o: ../libocr/GGraphOCR/GGraphOCRBackwardCorrelation.cpp
	$(CXX) ../libocr/GGraphOCR/GGraphOCRBackwardCorrelation.cpp -c -fPIC
GGraphOCRStarCorrelation.o: ../libocr/GGraphOCR/GGraphOCRStarCorrelation.cpp
	$(CXX) ../libocr/GGraphOCR/GGraphOCRStarCorrelation.cpp -c -fPIC
GGraphOCRCorrelationlookup2.o: ../libocr/GGraphOCR/GGraphOCRCorrelationlookup2.cpp
	$(CXX) ../libocr/GGraphOCR/GGraphOCRCorrelationlookup2.cpp -c -fPIC
GGraphOCRCorrelationlookup3.o: ../libocr/GGraphOCR/GGraphOCRCorrelationlookup3.cpp
	$(CXX) ../libocr/GGraphOCR/GGraphOCRCorrelationlookup3.cpp -c -fPIC
GGraphOCRCorrelationlookup4.o: ../libocr/GGraphOCR/GGraphOCRCorrelationlookup4.cpp
	$(CXX) ../libocr/GGraphOCR/GGraphOCRCorrelationlookup4.cpp -c -fPIC
GGraphOCRTextCorrelation.o: ../libocr/GGraphOCR/GGraphOCRTextCorrelation.cpp
	$(CXX) ../libocr/GGraphOCR/GGraphOCRTextCorrelation.cpp -c -fPIC
GGraphOCRTextCorrelationHash.o: ../libocr/GGraphOCR/GGraphOCRTextCorrelationHash.cpp
	$(CXX) ../libocr/GGraphOCR/GGraphOCRTextCorrelationHash.cpp -c -fPIC
GGraphOCRTextOCR.o: ../libocr/GGraphOCR/GGraphOCRTextOCR.cpp
	$(CXX) ../libocr/GGraphOCR/GGraphOCRTextOCR.cpp -c -fPIC

GDraw.o: ../libocr/GDraw/GDraw.cpp
	$(CXX) ../libocr/GDraw/GDraw.cpp -c -fPIC
GDraw_Graph.o: ../libocr/GDraw/GDraw_Graph.cpp
	$(CXX) ../libocr/GDraw/GDraw_Graph.cpp -c -fPIC

	
GImageEditor.o: ../libocr/GImageEditor/GImageEditor.cpp
	$(CXX) ../libocr/GImageEditor/GImageEditor.cpp -c -fPIC
GImageEditor_functions.o: ../libocr/GImageEditor/GImageEditor_functions.cpp
	$(CXX) ../libocr/GImageEditor/GImageEditor_functions.cpp -c -fPIC
GImageEditorAdaptiveFiltration.o: ../libocr/GImageEditor/GImageEditorAdaptiveFiltration.cpp
	$(CXX) ../libocr/GImageEditor/GImageEditorAdaptiveFiltration.cpp -c -fPIC
imageProcessOCR.o: ../libocr/GImageEditorOCR/imageProcessOCR.cpp
	$(CXX) ../libocr/GImageEditorOCR/imageProcessOCR.cpp -c -fPIC

	

GGraphBezier.o: ../libocr/GGraphBezier/GGraphBezier.cpp
	$(CXX) ../libocr/GGraphBezier/GGraphBezier.cpp -c -fPIC
GGraphBuildLetterSaBezier.o: ../libocr/GGraphBezier/GGraphBuildLetterSaBezier.cpp
	$(CXX) ../libocr/GGraphBezier/GGraphBuildLetterSaBezier.cpp -c -fPIC
GGraphCorrelationBezierFullBezier.o: ../libocr/GGraphBezier/GGraphCorrelationBezierFullBezier.cpp
	$(CXX) ../libocr/GGraphBezier/GGraphCorrelationBezierFullBezier.cpp -c -fPIC
GGraphCorrelationBezierPartBezier.o: ../libocr/GGraphBezier/GGraphCorrelationBezierPartBezier.cpp
	$(CXX) ../libocr/GGraphBezier/GGraphCorrelationBezierPartBezier.cpp -c -fPIC
GGraphFocalLineBezier.o: ../libocr/GGraphBezier/GGraphFocalLineBezier.cpp
	$(CXX) ../libocr/GGraphBezier/GGraphFocalLineBezier.cpp -c -fPIC
GGraphFocalLineRecLevelingBezier.o: ../libocr/GGraphBezier/GGraphFocalLineRecLevelingBezier.cpp
	$(CXX) ../libocr/GGraphBezier/GGraphFocalLineRecLevelingBezier.cpp -c -fPIC
GGraphFocalLineRecognitionBezier.o: ../libocr/GGraphBezier/GGraphFocalLineRecognitionBezier.cpp
	$(CXX) ../libocr/GGraphBezier/GGraphFocalLineRecognitionBezier.cpp -c -fPIC
GGraphFocalPointBezier.o: ../libocr/GGraphBezier/GGraphFocalPointBezier.cpp
	$(CXX) ../libocr/GGraphBezier/GGraphFocalPointBezier.cpp -c -fPIC
GGraphLineMatrixBezier.o: ../libocr/GGraphBezier/GGraphLineMatrixBezier.cpp
	$(CXX) ../libocr/GGraphBezier/GGraphLineMatrixBezier.cpp -c -fPIC
GGraphStableFocalPointBezier.o: ../libocr/GGraphBezier/GGraphStableFocalPointBezier.cpp
	$(CXX) ../libocr/GGraphBezier/GGraphStableFocalPointBezier.cpp -c -fPIC
GGraphTestBezier.o: ../libocr/GGraphBezier/GGraphTestBezier.cpp
	$(CXX) ../libocr/GGraphBezier/GGraphTestBezier.cpp -c -fPIC
	
#OCRLib_GGraphOCR.o: ../OCRLib_main/OCRLib_GGraphOCR.cpp
#	$(CXX) ../OCRLib_main/OCRLib_GGraphOCR.cpp -c -fPIC
OCRLib.o: ../OCRLib_main/OCRLib.cpp
	$(CXX) ../OCRLib_main/OCRLib.cpp -c
OCRLib_Editor.o: ../OCRLib_main/OCRLib_Editor.cpp
	$(CXX) ../OCRLib_main/OCRLib_Editor.cpp -c
OCRLib_Image.o: ../OCRLib_main/OCRLib_Image.cpp
	$(CXX) ../OCRLib_main/OCRLib_Image.cpp -c
OCRLib_OCR.o: ../OCRLib_main/OCRLib_OCR.cpp
	$(CXX) ../OCRLib_main/OCRLib_OCR.cpp -c
	
	
GBitmask.o: ../libocr/GBitMask/GBitmask.cpp
	$(CXX) ../libocr/GBitMask/GBitmask.cpp -c
algorithmOCR_128.o: ../libocr/GBitMask/algorithmOCR_128.cpp
	$(CXX) ../libocr/GBitMask/algorithmOCR_128.cpp -c
algorithmOCR_32.o: ../libocr/GBitMask/algorithmOCR_32.cpp
	$(CXX) ../libocr/GBitMask/algorithmOCR_32.cpp -c
matrixT.o: ../libocr/GBitMask/matrixT.cpp
	$(CXX) ../libocr/GBitMask/matrixT.cpp -c
	


GBitset.o: ../libocr/GBitset/GBitset.cpp
	$(CXX) ../libocr/GBitset/GBitset.cpp -c
GBitsetCorrectSqH.o: ../libocr/GBitset/GBitsetCorrectSqH.cpp
	$(CXX) ../libocr/GBitset/GBitsetCorrectSqH.cpp -c
GBitsetCorrectSqL.o: ../libocr/GBitset/GBitsetCorrectSqL.cpp
	$(CXX) ../libocr/GBitset/GBitsetCorrectSqL.cpp -c
GBitsetNet.o: ../libocr/GBitset/GBitsetNet.cpp
	$(CXX) ../libocr/GBitset/GBitsetNet.cpp -c
GBitsetPoint.o: ../libocr/GBitset/GBitsetPoint.cpp
	$(CXX) ../libocr/GBitset/GBitsetPoint.cpp -c
GBitsetStructure.o: ../libocr/GBitset/GBitsetStructure.cpp
	$(CXX) ../libocr/GBitset/GBitsetStructure.cpp -c
GBitsetTest.o: ../libocr/GBitset/GBitsetTest.cpp
	$(CXX) ../libocr/GBitset/GBitsetTest.cpp -c
GBitsetWord.o: ../libocr/GBitset/GBitsetWord.cpp
	$(CXX) ../libocr/GBitset/GBitsetWord.cpp -c
GBitsetWordEng.o: ../libocr/GBitset/GBitsetWordEng.cpp
	$(CXX) ../libocr/GBitset/GBitsetWordEng.cpp -c
GBitsetWordTib.o: ../libocr/GBitset/GBitsetWordTib.cpp
	$(CXX) ../libocr/GBitset/GBitsetWordTib.cpp -c
GBitset_Letter.o: ../libocr/GBitset/GBitset_Letter.cpp
	$(CXX) ../libocr/GBitset/GBitset_Letter.cpp -c
GBitset_Starting.o: ../libocr/GBitset/GBitset_Starting.cpp
	$(CXX) ../libocr/GBitset/GBitset_Starting.cpp -c
GBitset_algorithm.o: ../libocr/GBitset/GBitset_algorithm.cpp
	$(CXX) ../libocr/GBitset/GBitset_algorithm.cpp -c
GBitset_base.o: ../libocr/GBitset/GBitset_base.cpp
	$(CXX) ../libocr/GBitset/GBitset_base.cpp -c
GBitset_level.o: ../libocr/GBitset/GBitset_level.cpp
	$(CXX) ../libocr/GBitset/GBitset_level.cpp -c
GBitsetPageSegmentation.o: ../libocr/GBitset/GBitsetPageSegmentation.cpp
	$(CXX) ../libocr/GBitset/GBitsetPageSegmentation.cpp -c
	
GBitsetOCR.o: ../libocr/GBitsetOCR/GBitsetOCR.cpp
	$(CXX) ../libocr/GBitsetOCR/GBitsetOCR.cpp -c	
GBitset_baseOCR.o: ../libocr/GBitsetOCR/GBitset_baseOCR.cpp
	$(CXX) ../libocr/GBitsetOCR/GBitset_baseOCR.cpp -c
GBitset_setDataOCR.o: ../libocr/GBitsetOCR/GBitset_setDataOCR.cpp
	$(CXX) ../libocr/GBitsetOCR/GBitset_setDataOCR.cpp -c
lineOCR.o: ../libocr/GBitsetOCR/lineOCR.cpp
	$(CXX) ../libocr/GBitsetOCR/lineOCR.cpp -c
pageOCR.o: ../libocr/GBitsetOCR/pageOCR.cpp
	$(CXX) ../libocr/GBitsetOCR/pageOCR.cpp -c
setImageParamOCR.o: ../libocr/GBitsetOCR/setImageParamOCR.cpp
	$(CXX) ../libocr/GBitsetOCR/setImageParamOCR.cpp -c
setOCRCorrelation.o: ../libocr/GBitsetOCR/setOCRCorrelation.cpp
	$(CXX) ../libocr/GBitsetOCR/setOCRCorrelation.cpp -c
setOCRRecordCorrelation.o: ../libocr/GBitsetOCR/setOCRRecordCorrelation.cpp
	$(CXX) ../libocr/GBitsetOCR/setOCRRecordCorrelation.cpp -c
setScaleBitsetOCR.o: ../libocr/GBitsetOCR/setScaleBitsetOCR.cpp
	$(CXX) ../libocr/GBitsetOCR/setScaleBitsetOCR.cpp -c
setScaleRecord.o: ../libocr/GBitsetOCR/setScaleRecord.cpp
	$(CXX) ../libocr/GBitsetOCR/setScaleRecord.cpp -c
textOCR.o: ../libocr/GBitsetOCR/textOCR.cpp
	$(CXX) ../libocr/GBitsetOCR/textOCR.cpp -c
	
GContour.o: ../libocr/GFontEditor/GContour.cpp
	$(CXX) ../libocr/GFontEditor/GContour.cpp -c
GFont.o: ../libocr/GFontEditor/GFont.cpp
	$(CXX) ../libocr/GFontEditor/GFont.cpp -c
GFontEditor.o: ../libocr/GFontEditor/GFontEditor.cpp
	$(CXX) ../libocr/GFontEditor/GFontEditor.cpp -c
GLetter.o: ../libocr/GFontEditor/GLetter.cpp
	$(CXX) ../libocr/GFontEditor/GLetter.cpp -c
buildLetter.o: ../libocr/GFontEditor/buildLetter.cpp
	$(CXX) ../libocr/GFontEditor/buildLetter.cpp -c
drawBase.o: ../libocr/GFontEditor/drawBase.cpp
	$(CXX) ../libocr/GFontEditor/drawBase.cpp -c
letterOCR.o: ../libocr/GFontEditor/letterOCR.cpp
	$(CXX) ../libocr/GFontEditor/letterOCR.cpp -c
setLetter.o: ../libocr/GFontEditor/setLetter.cpp
	$(CXX) ../libocr/GFontEditor/setLetter.cpp -c
setLetterScale.o: ../libocr/GFontEditor/setLetterScale.cpp
	$(CXX) ../libocr/GFontEditor/setLetterScale.cpp -c
testLetter.o: ../libocr/GFontEditor/testLetter.cpp
	$(CXX) ../libocr/GFontEditor/testLetter.cpp -c
		
processInput.o: ../libocr/processInput.cpp
	$(CXX) ../libocr/processInput.cpp -c	
	
GBitsetMatrix_base.o: ../libocr/GBitsetMatrix/GBitsetMatrix_base.cpp
	$(CXX) ../libocr/GBitsetMatrix/GBitsetMatrix_base.cpp -c
GBitsetMatrix_level.o: ../libocr/GBitsetMatrix/GBitsetMatrix_level.cpp
	$(CXX) ../libocr/GBitsetMatrix/GBitsetMatrix_level.cpp -c
MartixCorrelation.o: ../libocr/GBitsetMatrix/MartixCorrelation.cpp
	$(CXX) ../libocr/GBitsetMatrix/MartixCorrelation.cpp -c
Matrix_Lion.o: ../libocr/GBitsetMatrix/Matrix_Lion.cpp
	$(CXX) ../libocr/GBitsetMatrix/Matrix_Lion.cpp -c
letterCorrelation.o: ../libocr/GBitsetMatrix/letterCorrelation.cpp
	$(CXX) ../libocr/GBitsetMatrix/letterCorrelation.cpp -c
letterToLetterCorrelation.o: ../libocr/GBitsetMatrix/letterToLetterCorrelation.cpp
	$(CXX) ../libocr/GBitsetMatrix/letterToLetterCorrelation.cpp -c
setMatrix_Point_32_64C.o: ../libocr/GBitsetMatrix/setMatrix_Point_32_64C.cpp
	$(CXX) ../libocr/GBitsetMatrix/setMatrix_Point_32_64C.cpp -c
setMatrix_Point_32_64_SSE.o: ../libocr/GBitsetMatrix/setMatrix_Point_32_64_SSE.cpp
	$(CXX) ../libocr/GBitsetMatrix/setMatrix_Point_32_64_SSE.cpp -c
setMatrix_Point_64ASM_ATT.o: ../libocr/GBitsetMatrix/setMatrix_Point_64ASM_ATT.cpp
	$(CXX) ../libocr/GBitsetMatrix/setMatrix_Point_64ASM_ATT.cpp -c
setMatrix_Point_64ASM_INTEL_F.o: ../libocr/GBitsetMatrix/setMatrix_Point_64ASM_INTEL_F.cpp
	$(CXX) ../libocr/GBitsetMatrix/setMatrix_Point_64ASM_INTEL_F.cpp -c
setMatrix_Point_64_popcnt_C.o: ../libocr/GBitsetMatrix/setMatrix_Point_64_popcnt_C.cpp
	$(CXX) ../libocr/GBitsetMatrix/setMatrix_Point_64_popcnt_C.cpp -c
	
	
GLogicProcessor.o: ../libocr/GLogicProcessor/GLogicProcessor.cpp
	$(CXX) ../libocr/GLogicProcessor/GLogicProcessor.cpp -c
LoadFontMaps.o: ../libocr/GLogicProcessor/LoadFontMaps.cpp
	$(CXX) ../libocr/GLogicProcessor/LoadFontMaps.cpp -c
UniToYagpo.o: ../libocr/GLogicProcessor/UniToYagpo.cpp
	$(CXX) ../libocr/GLogicProcessor/UniToYagpo.cpp -c
buidSearchString.o: ../libocr/GLogicProcessor/buidSearchString.cpp
	$(CXX) ../libocr/GLogicProcessor/buidSearchString.cpp -c
buidSearchStringNew.o: ../libocr/GLogicProcessor/buidSearchStringNew.cpp
	$(CXX) ../libocr/GLogicProcessor/buidSearchStringNew.cpp -c
buildOpenType.o: ../libocr/GLogicProcessor/buildOpenType.cpp
	$(CXX) ../libocr/GLogicProcessor/buildOpenType.cpp -c
classification.o: ../libocr/GLogicProcessor/classification.cpp
	$(CXX) ../libocr/GLogicProcessor/classification.cpp -c
codePagesConverter.o: ../libocr/GLogicProcessor/codePagesConverter.cpp
	$(CXX) ../libocr/GLogicProcessor/codePagesConverter.cpp -c
collectStackLetter.o: ../libocr/GLogicProcessor/collectStackLetter.cpp
	$(CXX) ../libocr/GLogicProcessor/collectStackLetter.cpp -c
collectTextLine.o: ../libocr/GLogicProcessor/collectTextLine.cpp
	$(CXX) ../libocr/GLogicProcessor/collectTextLine.cpp -c
compressMatch.o: ../libocr/GLogicProcessor/compressMatch.cpp
	$(CXX) ../libocr/GLogicProcessor/compressMatch.cpp -c
converterPali.o: ../libocr/GLogicProcessor/converterPali.cpp
	$(CXX) ../libocr/GLogicProcessor/converterPali.cpp -c
dictReport.o: ../libocr/GLogicProcessor/dictReport.cpp
	$(CXX) ../libocr/GLogicProcessor/dictReport.cpp -c
dictReportLocal.o: ../libocr/GLogicProcessor/dictReportLocal.cpp
	$(CXX) ../libocr/GLogicProcessor/dictReportLocal.cpp -c
drawGrapeLine.o: ../libocr/GLogicProcessor/drawGrapeLine.cpp
	$(CXX) ../libocr/GLogicProcessor/drawGrapeLine.cpp -c
drawMatchLine.o: ../libocr/GLogicProcessor/drawMatchLine.cpp
	$(CXX) ../libocr/GLogicProcessor/drawMatchLine.cpp -c
grammarCorrector.o: ../libocr/GLogicProcessor/grammarCorrector.cpp
	$(CXX) ../libocr/GLogicProcessor/grammarCorrector.cpp -c
grammarOCR.o: ../libocr/GLogicProcessor/grammarOCR.cpp
	$(CXX) ../libocr/GLogicProcessor/grammarOCR.cpp -c
intersectionArea.o: ../libocr/GLogicProcessor/intersectionArea.cpp
	$(CXX) ../libocr/GLogicProcessor/intersectionArea.cpp -c
letterAssociation.o: ../libocr/GLogicProcessor/letterAssociation.cpp
	$(CXX) ../libocr/GLogicProcessor/letterAssociation.cpp -c
letterConstructions.o: ../libocr/GLogicProcessor/letterConstructions.cpp
	$(CXX) ../libocr/GLogicProcessor/letterConstructions.cpp -c
letterNeighbors.o: ../libocr/GLogicProcessor/letterNeighbors.cpp
	$(CXX) ../libocr/GLogicProcessor/letterNeighbors.cpp -c
letterNeighborsNew.o: ../libocr/GLogicProcessor/letterNeighborsNew.cpp
	$(CXX) ../libocr/GLogicProcessor/letterNeighborsNew.cpp -c
letterNeighborsSanskrit.o: ../libocr/GLogicProcessor/letterNeighborsSanskrit.cpp
	$(CXX) ../libocr/GLogicProcessor/letterNeighborsSanskrit.cpp -c
letterProbability.o: ../libocr/GLogicProcessor/letterProbability.cpp
	$(CXX) ../libocr/GLogicProcessor/letterProbability.cpp -c
loadDictionary.o: ../libocr/GLogicProcessor/loadDictionary.cpp
	$(CXX) ../libocr/GLogicProcessor/loadDictionary.cpp -c
loadMap.o: ../libocr/GLogicProcessor/loadMap.cpp
	$(CXX) ../libocr/GLogicProcessor/loadMap.cpp -c
pageSegmentation.o: ../libocr/GLogicProcessor/pageSegmentation.cpp
	$(CXX) ../libocr/GLogicProcessor/pageSegmentation.cpp -c
renderDictSearch.o: ../libocr/GLogicProcessor/renderDictSearch.cpp
	$(CXX) ../libocr/GLogicProcessor/renderDictSearch.cpp -c
renderDictSearchNew.o: ../libocr/GLogicProcessor/renderDictSearchNew.cpp
	$(CXX) ../libocr/GLogicProcessor/renderDictSearchNew.cpp -c
renderDictSearchSanskrit.o: ../libocr/GLogicProcessor/renderDictSearchSanskrit.cpp
	$(CXX) ../libocr/GLogicProcessor/renderDictSearchSanskrit.cpp -c
sentenceConstructor.o: ../libocr/GLogicProcessor/sentenceConstructor.cpp
	$(CXX) ../libocr/GLogicProcessor/sentenceConstructor.cpp -c
sentenceConstructorNew.o: ../libocr/GLogicProcessor/sentenceConstructorNew.cpp
	$(CXX) ../libocr/GLogicProcessor/sentenceConstructorNew.cpp -c
spellChecker.o: ../libocr/GLogicProcessor/spellChecker.cpp
	$(CXX) ../libocr/GLogicProcessor/spellChecker.cpp -c
startConvert.o: ../libocr/GLogicProcessor/startConvert.cpp
	$(CXX) ../libocr/GLogicProcessor/startConvert.cpp -c
startDictionary.o: ../libocr/GLogicProcessor/startDictionary.cpp
	$(CXX) ../libocr/GLogicProcessor/startDictionary.cpp -c
testStack.o: ../libocr/GLogicProcessor/testStack.cpp
	$(CXX) ../libocr/GLogicProcessor/testStack.cpp -c
testWordLine.o: ../libocr/GLogicProcessor/testWordLine.cpp
	$(CXX) ../libocr/GLogicProcessor/testWordLine.cpp -c
	
GMainEditor.o: ../libocr/GMainEditor/GMainEditor.cpp
	$(CXX) ../libocr/GMainEditor/GMainEditor.cpp -c
drawPage.o: ../libocr/GMainEditor/drawPage.cpp
	$(CXX) ../libocr/GMainEditor/drawPage.cpp -c
readInputAction.o: ../libocr/GMainEditor/readInputAction.cpp
	$(CXX) ../libocr/GMainEditor/readInputAction.cpp -c
setImageOCR.o: ../libocr/GMainEditor/setImageOCR.cpp
	$(CXX) ../libocr/GMainEditor/setImageOCR.cpp -c
setScaleOCR.o: ../libocr/GMainEditor/setScaleOCR.cpp
	$(CXX) ../libocr/GMainEditor/setScaleOCR.cpp -c
startOCR.o: ../libocr/GMainEditor/startOCR.cpp
	$(CXX) ../libocr/GMainEditor/startOCR.cpp -c
vectorOCR.o: ../libocr/GMainEditor/vectorOCR.cpp
	$(CXX) ../libocr/GMainEditor/vectorOCR.cpp -c

