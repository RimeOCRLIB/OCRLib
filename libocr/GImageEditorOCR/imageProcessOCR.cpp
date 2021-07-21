#include <errno.h>
#include "../GFontEditor/GLetter.h"
#include "../GMainEditor/GMainEditor.h"

namespace ocr{

void proccessImageOCR(GBitmap *pechaImg, float *param) {
  if(inputData.data["action"] == "splitPages") inputData.data["SplitPage"] = "splitPage";

  vector<stringOCR> strArray;
  if(inputData.data["SplitPage"] == "splitPage") {
    cout << "SplitPage" << endl;
    strArray.resize(0);
    int      x = pechaImg->columns() / 6;
    int      y = 0;
    int      w = (pechaImg->columns() * 4) / 6;
    int      h = pechaImg->rows();
    GBitmap *pechaImgResize = GBitmap::createRegion(pechaImg, x, y, w, h);
    GBitmap *pechaImgRotate = GBitmap::createRotate(pechaImgResize, -90);
    pechaImgResize->destroy();
    float angle = pechaImgRotate->detectPageAngle(8);
    pechaImgRotate->destroy();
    pechaImgRotate = GBitmap::createRotate(pechaImg, -angle);

    GBitset *setBit = GBitset::createResize(pechaImgRotate, 1, 1, 1);
    setBit->SplitPagesBtn = 1;
    setBit->pageStringDetector(strArray, 2);
    cout << "TurnXL=" << setBit->TurnX << " border=" << setBit->border_size() << " w=" << setBit->columns() << endl;
    // WriteImageData(setBit->drawData,"/_Image2OCR/1.jpg", 1); //exit(0);
    string st = inputData.data["inputFile"];
    string name = st.erase(0, st.rfind('/'));
    string ext = name;
    ext = ext.erase(0, ext.rfind('.'));
    name = name.erase(name.size() - ext.size(), ext.size());
    string folderName = inputData.data["inputFile"];
    folderName = folderName.erase(folderName.rfind('/'), folderName.size());  // cout_<<" st@@@"<<folderName<<endl;
    st = folderName;
    st += "/";
    st += name;
    st += "_0";
    st += ext;
    WriteSpreadData(pechaImgRotate, setBit, -1, st);  // , strArray
    st = folderName;
    st += "/";
    st += name;
    st += "_1";
    st += ext;                                        // cout_<<" st"<<st<<endl;
    WriteSpreadData(pechaImgRotate, setBit, -2, st);  // , strArray
    pechaImgRotate->destroy();
    setBit->destroy();
    return;
  }
  if(inputData.data["SplitPage"] == "splitColumns") {
    /*Для определения середины страницы вырезаем ценральную область страницы 2/3 ширины)
     затем поворачиваем на 90 градучов и исправляем наклон, определяя его по центральной области страницы
     всю страницу поворачиваем на нужный угол. В получившейся ровной странице ищем разделение на колонки.
     Лучшие результаты при наличии разделительной линии между колонками.
     */
    cout << "splitColumns" << endl;
    strArray.resize(0);
    int      x = pechaImg->columns() / 6;
    int      y = 0;
    int      w = (pechaImg->columns() * 4) / 6;
    int      h = pechaImg->rows();
    GBitmap *pechaImgResize = GBitmap::createRegion(pechaImg, x, y, w, h);
    GBitmap *pechaImgRotate = GBitmap::createRotate(pechaImgResize, -90);
    pechaImgResize->destroy();
    float angle = pechaImgRotate->detectPageAngle(8);
    pechaImgRotate->destroy();
    pechaImgRotate = GBitmap::createRotate(pechaImg, -angle);
    // WriteImageData(pechaImgRotate,"/test.jpg", 1);

    GBitset *setBit = GBitset::createResize(pechaImgRotate, 1, 1, 1);
    setBit->SplitPagesBtn = 1;
    setBit->pageStringDetector(strArray, 1);
#ifdef DEBUGLVL_GREY
    WriteImageData(setBit->drawData, "/1.jpg", 0);
#endif
#ifdef DEBUGLVL_DRAW
    WriteImageData(setBit->drawDataRGB, "/1.jpg", 0);
#endif

    //  ДЛЯ ПОКАЗА КАРТИНКИ
    // if(CheckBoxTIFF->Checked==true){ext=".tif";}
    // if(AutoRemoveBorderBtn->Checked==true){
    string st = inputData.data["inputFile"];
    string name = st.erase(0, st.rfind('/'));
    string ext = name;
    ext = ext.erase(0, ext.rfind('.'));
    name = name.erase(name.size() - ext.size(), ext.size());
    st = inputData.data["inputFile"];
    st.erase(st.rfind('/'), st.size());
    st += "/";
    inputData.data["outputFolder"] = st;
    st += name;
    st += "_0";
    st += ext;
    cout << " st" << st << endl;
    WriteSpreadData(pechaImgRotate, setBit, -1, st);  // , strArray
    st = inputData.data["outputFolder"];
    st += name;
    st += "_1";
    st += ext;
    cout << " st" << st << endl;
    WriteSpreadData(pechaImgRotate, setBit, -2, st);  // , strArray
    setBit->destroy();
    pechaImgRotate->destroy();
    cout << "done split";
    exit(0);
  }

  if(inputData.data["SplitPage"] == "eraseBorder") {
    int pageBorderX = atoi(inputData.data["BorderX"].c_str());
    int pageBorderY = atoi(inputData.data["BorderY"].c_str());

    cout << "pageBorderX=" << pageBorderX << "pageBorderY=" << pageBorderY << endl;
    pechaImg->eraseBorder(pageBorderX, pageBorderY);  // , strArray
  }

  // cout_<<"inputData.data[\"rotationBox\"]"<<inputData.data["rotationBox"]<<endl;

  float scale;
  if(inputData.data["scale"] != "") {
    scale = atoi(inputData.data["scale"].c_str());
    scale /= 100;
    float p[] = {scale, 85, 14, 0, 0, 0, 47, 1, 1, 0, 0};
    param = p;
  } else {
    scale = param[0];
  }

  if(inputData.data["action"] == "baseNormalisation") { adaptiveFiltration1(pechaImg, param, 1); }

  if(inputData.data["action"] == "adaptiveFiltration") {
    int   Tr = atoi(inputData.data["tr"].c_str());
    int   Base = atoi(inputData.data["base"].c_str());
    int   Blur = atoi(inputData.data["blur"].c_str());
    int   DotGain = atoi(inputData.data["dotgain"].c_str());
    int   Erosion = atoi(inputData.data["erosion"].c_str());
    float p[] = {scale, (float)Tr, (float)Base, 0, (float)Blur, 0, 47, (float)Erosion, (float)DotGain, 12, 0};
    param = p;
    adaptiveFiltration1(pechaImg, param, 2);
  }
  if(inputData.data["action"] == "OCR_normalisation") { adaptiveFiltration1(pechaImg, param, 2); }

  if(inputData.data["action"] == "alignContent" || inputData.data["action"] == "crop") {
    /*Для выравнивания страницы:
     0. Стираем мусор по краю страницы
     1. Уменьшаем страницу в 3 раза для оптимизации выделения строк.
     2. Определяем верхнюю и нижнюю строки
     3. Поворачиваем страницу на 90 градусов
     4. Определяем левую и правую границу текста
     5. Рисуем центральную область по центру пустой страницы*/
    cout << "alignContent" << endl;
    strArray.resize(0);

    pechaImg->eraseBorder(pechaImg->columns() / 30, pechaImg->rows() / 30);  // , strArray

    GBitmap *pechaImgResize = GBitmap::createScale(pechaImg, 0.5);
    pechaImgResize->gaussian(5);
    GBitmap *pechaImgRotate = GBitmap::createRotate(pechaImgResize, -90);

    GBitset *setBit = GBitset::createResize(pechaImgResize, 1, 1, 1);
    setBit->pageStringDetector(strArray, 1);  // Подпрограмма управления последовательностью выполнения всех подпрограмм (их 7 штук).
    // WriteImageData(pechaImgResize,"/_Image2OCR/1.jpg", 0); exit(0);

    if(!strArray.size()) return;
    int pageY0 = strArray[strArray.size() - 1].y0 - 140;
    pageY0 *= 2;
    if(pageY0 < 0) pageY0 = 0;
    int pageY1 = strArray[0].y1 + 140;
    pageY1 *= 2;
    if(pageY1 > pechaImg->rows()) pageY1 = pechaImg->rows();

#ifdef DEBUGLVL_GREY
    WriteImageData(setBit->drawData, "/_Image2OCR/1.jpg", 0);
#endif
#ifdef DEBUGLVL_DRAW
    WriteImageData(setBit->drawDataRGB, "/_Image2OCR/1.jpg", 0);
#endif

    int pageH = pageY1 - pageY0;
    strArray.resize(0);
    setBit->destroy();
    setBit = GBitset::createResize(pechaImgRotate, 1, 1, 0);
    setBit->pageStringDetector(strArray, 1);

    if(!strArray.size()) return;
    int pageX0 = strArray[strArray.size() - 1].y0 - 50;
    pageX0 *= 2;
    if(pageX0 < 0) pageX0 = 0;
    int pageX1 = strArray[0].y1 + 50;
    pageX1 *= 2;
    if(pageX1 > pechaImg->columns()) pageX0 = pechaImg->columns();
    int      pageW = pechaImg->columns();
    GBitmap *pageContent = GBitmap::createRegion(pechaImg, 0, pageY0, pageW, pageH);
    pechaImg->fill(255);
    if(inputData.data["action"] == "alignContent") {
      pechaImg->drawImg(pageContent, 0, (pechaImg->rows() - pageH) / 2);
      pageContent->destroy();
    }
    if(inputData.data["action"] == "crop") {
      pechaImg->destroy();
      pechaImg = pageContent;
    }
    setBit->destroy();
    pechaImgRotate->destroy();
    pechaImgResize->destroy();
  }
}

//основная функция графической обработки книги для OCR
void setImageByOCR(GBitmap *pechaImg, float *param_, int mode) {
  int print = 0;

  float scale;
  scale = ((GMainEditor *)inputData.mainEditor)->setScaleOCR(pechaImg);
  DR(" scale=" << scale << endl)
  if(mode == 1) {  //режим определения масштаба
    param_[0] = scale;
    cout << "@@__scale=" << scale << endl;
    return;
  }
  if(scale == -1) {
    param_[0] = -1;
    cout << "@no scale detected" << endl;
    return;
  }

  //выполняем подбор параметров графической обработки
  /*
   p[0] - scale
   p[1] - Tr1=85;
   p[2] - BASE1=24; ///70 светлее 90 темнее BASE1=32 для Калачакры с равной толщиной штриха, было 4
   p[3] - amplitude0=0;     /// 2 для Калачакры с равной толщиной штриха, было 0
   p[4] - amplitude1=2;  // amplitude=4  // 0 без изменений.
   p[5] - textOrientation=0;
   p[6] - line_wd1=47;  //line_wd1=51;  // если line_wd1=0  то оно вычисляется функцией измерения толщины штриха 47
   p[7] - n_RemovLayers=1;
   p[8] - dot_gain=1;
   p[9] - g; //диапазон угола детектора поворота
   p[10]- //нормализация ориентации
   */
  float           BaseTr = 24;
  float           Tr = 85;
  float           rT = 12;
  int             index = -1;
  vector<float *> param;
  OCRPointFloat   point;  //структура для возврата значений графической обработки

  // int maxCor=0;
  // int maxTr=-1;
  // int maxBase=-1;

  BaseTr = -1;  //начальная площадь усреднения выбирается по высоте строки
  //определяем параметры порога усреденения
  float p0_2[] = {scale, 85, BaseTr, 0, 2, 0, 47, 1, 1, rT, 0};  //стандартные параметры
  float p1_2[] = {scale, 75, BaseTr, 0, 2, 0, 47, 1, 1, rT, 0};  //светлее
  float p2_2[] = {scale, 77, BaseTr, 0, 2, 0, 47, 1, 1, rT, 0};  //светлее
  float p3_2[] = {scale, 80, BaseTr, 0, 2, 0, 47, 1, 1, rT, 0};  //светлее
  float p4_2[] = {scale, 82, BaseTr, 0, 2, 0, 47, 1, 1, rT, 0};  //светлее
  float p5_2[] = {scale, 87, BaseTr, 0, 2, 0, 47, 1, 1, rT, 0};  //темнее
  float p6_2[] = {scale, 90, BaseTr, 0, 2, 0, 47, 1, 1, rT, 0};  //темнее
  float p7_2[] = {scale, 93, BaseTr, 0, 2, 0, 47, 1, 1, rT, 0};  //темнее
  float p8_2[] = {scale, 95, BaseTr, 0, 2, 0, 47, 1, 1, rT, 0};  //темнее
  float p9_2[] = {scale, 97, BaseTr, 0, 2, 0, 47, 1, 1, rT, 0};  //темнее

  param.resize(0);
  param.push_back(p0_2);
  param.push_back(p1_2);
  param.push_back(p2_2);
  param.push_back(p3_2);
  param.push_back(p4_2);
  param.push_back(p5_2);
  param.push_back(p6_2);
  param.push_back(p7_2);
  param.push_back(p8_2);
  param.push_back(p9_2);
  //сначала определяем лучшие параметры бинаризации
  index = ((GMainEditor *)inputData.mainEditor)->setImageOCR(pechaImg, param, point);
  if(index > -1) {
    Tr = param[index][1];
  } else {
    BaseTr = param[0][2];
  }
  DR(" index=" << index << " BaseTr=" << BaseTr << " Tr=" << Tr << endl)

  //определяем параметры площади усреденения
  float p0_1[] = {scale, Tr, 16, 0, 2, 0, 47, 1, 1, rT, 0};  //стандартные параметры
  float p1_1[] = {scale, Tr, 8, 0, 2, 0, 47, 1, 1, rT, 0};   //
  float p2_1[] = {scale, Tr, 10, 0, 2, 0, 47, 1, 1, rT, 0};  //
  float p3_1[] = {scale, Tr, 12, 0, 2, 0, 47, 1, 1, rT, 0};  //
  float p4_1[] = {scale, Tr, 14, 0, 2, 0, 47, 1, 1, rT, 0};  //
  float p5_1[] = {scale, Tr, 18, 0, 2, 0, 47, 1, 1, rT, 0};  //
  float p6_1[] = {scale, Tr, 20, 0, 2, 0, 47, 1, 1, rT, 0};  //
  float p7_1[] = {scale, Tr, 24, 0, 2, 0, 47, 1, 1, rT, 0};  //
  float p8_1[] = {scale, Tr, 28, 0, 2, 0, 47, 1, 1, rT, 0};  //

  param.resize(0);
  param.push_back(p0_1);
  param.push_back(p1_1);
  param.push_back(p2_1);
  param.push_back(p3_1);
  param.push_back(p4_1);
  param.push_back(p5_1);
  param.push_back(p6_1);
  param.push_back(p7_1);
  param.push_back(p8_1);
  //сначала определяем лучшие параметры бинаризации
  index = ((GMainEditor *)inputData.mainEditor)->setImageOCR(pechaImg, param, point);
  if(index > -1) BaseTr = param[index][2];
  DR(" index=" << index << " BaseTr=" << BaseTr << " Tr=" << Tr << " cor=" << point.x << endl)

  //еще раз определяем параметры порога усреденения
  float p0_2_[] = {scale, 85, BaseTr, 0, 2, 0, 47, 1, 1, rT, 0};  //стандартные параметры
  float p1_2_[] = {scale, 75, BaseTr, 0, 2, 0, 47, 1, 1, rT, 0};  //светлее
  float p2_2_[] = {scale, 77, BaseTr, 0, 2, 0, 47, 1, 1, rT, 0};  //светлее
  float p3_2_[] = {scale, 80, BaseTr, 0, 2, 0, 47, 1, 1, rT, 0};  //светлее
  float p4_2_[] = {scale, 82, BaseTr, 0, 2, 0, 47, 1, 1, rT, 0};  //светлее
  float p5_2_[] = {scale, 87, BaseTr, 0, 2, 0, 47, 1, 1, rT, 0};  //темнее
  float p6_2_[] = {scale, 90, BaseTr, 0, 2, 0, 47, 1, 1, rT, 0};  //темнее
  float p7_2_[] = {scale, 93, BaseTr, 0, 2, 0, 47, 1, 1, rT, 0};  //темнее
  float p8_2_[] = {scale, 95, BaseTr, 0, 2, 0, 47, 1, 1, rT, 0};  //темнее
  float p9_2_[] = {scale, 97, BaseTr, 0, 2, 0, 47, 1, 1, rT, 0};  //темнее

  param.resize(0);
  param.push_back(p0_2_);
  param.push_back(p1_2_);
  param.push_back(p2_2_);
  param.push_back(p3_2_);
  param.push_back(p4_2_);
  param.push_back(p5_2_);
  param.push_back(p6_2_);
  param.push_back(p7_2_);
  param.push_back(p8_2_);
  param.push_back(p9_2_);
  //сначала определяем лучшие параметры бинаризации
  index = ((GMainEditor *)inputData.mainEditor)->setImageOCR(pechaImg, param, point);
  if(index > -1) Tr = param[index][1];
  DR(" index=" << index << " BaseTr=" << BaseTr << " Tr=" << Tr << endl)

  //определяем параметры функции Гауса
  float p0_3[] = {scale, Tr, BaseTr, 0, 0, 0, 47, 1, 1, rT, 0};  //без фильтра
  float p1_3[] = {scale, Tr, BaseTr, 0, 1, 0, 47, 1, 1, rT, 0};  //слабое сглаживание
  float p2_3[] = {scale, Tr, BaseTr, 0, 2, 0, 47, 1, 1, rT, 0};  //стандартное сглаживание
  float p3_3[] = {scale, Tr, BaseTr, 0, 3, 0, 47, 1, 1, rT, 0};  //активное сглаживание
  float p4_3[] = {scale, Tr, BaseTr, 0, 4, 0, 47, 1, 1, rT, 0};  //сильное сглаживание

  param.resize(0);
  param.push_back(p0_3);
  param.push_back(p1_3);
  param.push_back(p2_3);
  param.push_back(p3_3);
  param.push_back(p4_3);
  index = ((GMainEditor *)inputData.mainEditor)->setImageOCR(pechaImg, param, point);
  float pGauss = 2;
  if(index > -1) pGauss = param[index][4];
  DR(" index=" << index << " pGauss=" << pGauss << endl)

  //определяем параметры еррозии и депозиции (полиграфического растискивания)
  float p0_4[] = {scale, Tr, BaseTr, 0, 3, 0, 47, 0, 0, rT, 0};  //без применения фильтра
  float p1_4[] = {scale, Tr, BaseTr, 0, 1, 0, 47, 1, 1, rT, 0};  //стандартные параметры
  float p2_4[] = {scale, Tr, BaseTr, 0, 2, 0, 47, 0, 1, rT, 0};  //темнее
  float p3_4[] = {scale, Tr, BaseTr, 0, 3, 0, 47, 1, 0, rT, 0};  //светлее
  float p4_4[] = {scale, Tr, BaseTr, 0, 3, 0, 47, 2, 2, rT, 0};  //сильнее сглаживание
  float p5_4[] = {scale, Tr, BaseTr, 0, 3, 0, 47, 1, 2, rT, 0};  //сильнее сглаживание темнее
  float p6_4[] = {scale, Tr, BaseTr, 0, 3, 0, 47, 2, 1, rT, 0};  //сильнее сглаживание светлее

  param.resize(0);
  param.push_back(p0_4);
  param.push_back(p1_4);
  param.push_back(p2_4);
  param.push_back(p3_4);
  param.push_back(p4_4);
  param.push_back(p5_4);
  param.push_back(p6_4);

  index = ((GMainEditor *)inputData.mainEditor)->setImageOCR(pechaImg, param, point);
  float remLayers = 1;
  float dotGain = 1;
  if(index > -1) {
    remLayers = param[index][7];
    remLayers = param[index][8];
  }
  DR(" index=" << index << " remLayers=" << remLayers << " dotGain=" << dotGain << endl)
  print = 1;
  DR("@@__scale=" << scale << " Tr=" << Tr << " BaseTr=" << BaseTr << " pGauss=" << pGauss << " remLayers=" << remLayers
                  << " dotGain=" << dotGain << endl)

  //возвращаем параметры обработки
  param_[0] = scale;
  param_[1] = Tr;
  param_[2] = BaseTr;
  param_[3] = 0;
  param_[4] = pGauss;
  param_[5] = 0;
  param_[6] = 47;
  param_[7] = remLayers;
  param_[8] = dotGain;
  param_[9] = 12;
  param_[10] = 0;
}

void bookNormalisationOCR() {
  if(!inputData.fileList.size()) return;

  DIR *dir;
  if((dir = opendir(inputData.fileList[0].c_str())) != NULL) {
    // cout<<"NEW DIR#1 "<<inputData.fileList[0]<<endl;
    readDirectoryToArray(inputData.fileList, inputData.fileList[0], "img");
  }
  string testFile = inputData.fileList[(int)inputData.fileList.size() / 2];
  inputData.data["scale"] = "";
  cout << " #TEST File " << (int)inputData.fileList.size() / 2 << " from " << inputData.fileList.size() << " " << testFile << endl;
  //для тестового файла выполняем подбор масштаба
  GBitmap *pechaImg = GBitmap::create(testFile.c_str());

  if(pechaImg->data_size > 7000 * 7000) {
    cout << "image is too big. Are you sure it is scanned book page?";
    return;
  }
  // pechaImg->autoLevel();
  //определяем параметры обработки
  float pRes[11];
  ((GMainEditor *)inputData.mainEditor)->resetOCR();
  setImageByOCR(pechaImg, pRes, 2);

  for(int index = 1; index < 5; index++) {
    if(pRes[0] == -1) {
      cout << " new test" << endl;
      int indexPage = (int)inputData.fileList.size() / 2 + index * 5;
      if(indexPage > inputData.fileList.size()) indexPage = (int)inputData.fileList.size() / 2 - index;
      if(index < 0) break;
      testFile = inputData.fileList[indexPage];
      inputData.data["scale"] = "";
      cout << " #TEST File from " << inputData.fileList.size() << " " << testFile << endl;
      //для тестового файла выполняем подбор масштаба
      pechaImg->destroy();
      ((GMainEditor *)inputData.mainEditor)->resetOCR();
      pechaImg = GBitmap::create(testFile.c_str());
      // pechaImg->autoLevel();
      //определяем параметры обработки
      setImageByOCR(pechaImg, pRes, 2);
      if(pRes[0] != -1) break;
    }
  }

  if(pRes[0] == -1) {
    cout << "@@@ book scale not detected" << endl;
    return;
  }

  for(int i = 0; i < inputData.fileList.size(); i++) {
    if(!is_file(inputData.fileList[i])) continue;
    cout << "NEW START from " << inputData.fileList.size() << " inputData.fileList[" << i << "]=" << inputData.fileList[i] << endl;
    inputData.data["inputFile"] = inputData.fileList[i];
    pechaImg = GBitmap::create(inputData.data["inputFile"].c_str());
    proccessImageOCR(pechaImg, pRes);
    pechaImg->destroy();
  }
}

int forkImageProcess(pidID *pidIDArray, int maxFork, int ID) {
  GBitmap *pechaImg = GBitmap::create(inputData.data["inputFile"].c_str());
  int      pid = fork();
  if(pid < 0) {
    cout << "ERROR on fork ";
    pechaImg->destroy();
    return 0;
  }
  if(pid == 0) {
    cout_ << "//___@inputID=" << ID << endl;
    if(pechaImg != NULL) {
      float a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      proccessImageOCR(pechaImg, a);
    } else {
      cout << "@@@!!!ERROR IN IMAGE!!!@@@" << inputData.data["inputFile"] << endl;
    }
    pidIDArray[ID].status = 0;
    exit(0);
  }
  pechaImg->destroy();
  return 1;

}  //______________________________________________

void imageNormalisation(void *letter_) {
  GLetter &letter = *(GLetter *)letter_;
  // cout<<inputData.data["action"];
  // string path=inputData.data["root"]+"/edit/letterImg.jpg";
  if(inputData.data["action"] == "imageNormalisation") {
    if(!letter.maskOriginal.status || letter.maskOriginal.mW != 128) {
      if(letter.maskOriginal.mW != 128) cout << "new original" << endl;
      letter.maskOriginal = letter.mask128;  // save original mask
      letter.maskOriginal.status = 1;
      // cout<<" save original";
    } else {
      letter.mask128 = letter.maskOriginal;  // restore original mask
    }
    GBitmap *img = letter.mask128.unPackMask();
    int      w = img->columns();
    int      h = img->rows();
    GBitmap *outBitmap = GBitmap::create(PICT_SIZE, PICT_SIZE);
    int      x0 = letter.mask128.xMask + PICT_SIZE / 2;
    int      y0 = letter.mask128.yMask + PICT_SIZE / 2;
    outBitmap->drawImg(img, x0, y0);
    float param[] = {1, 85, 24, 0, 3, 0, 47, 1, 2, 0, 0};
    adaptiveFiltration1(outBitmap, param, 2);
    // string mode="jpg";
    // WriteImageData(outBitmap, path, 0,mode);
    img->destroy();
    img = GBitmap::createRegion(outBitmap, x0, y0, w, h);
    // WriteImageData(img, "/_1.jpg", 0);
    letter.mask128.packBitMask128(img);
    img->destroy();
    outBitmap->destroy();
    letter.reloadMask();
  }
  if(inputData.data["action"] == "revertImage") {
    cout << "letter.maskOriginal.status=" << (short)letter.maskOriginal.status;
    if(letter.maskOriginal.status) {
      letter.mask128 = letter.maskOriginal;  // restore original mask
    }
    letter.reloadMask();
  }
  if(inputData.data["action"] == "setFocalPoints") {
    vector<OCRPointFloat> str;
    GBitmap *             img = letter.mask128.unPackMask();
    GGraphBezier          outGraph = GGraphBezier(PICT_SIZE, PICT_SIZE);
    int                   x0 = letter.mask128.xMask + PICT_SIZE / 2;
    int                   y0 = letter.mask128.yMask + PICT_SIZE / 2;
    outGraph.drawImg(img, x0, y0);

    outGraph.StablePoint(str);

    // WriteImageData(outBitmap, "/_Image2OCR/_2.jpg", 0); //exit(0);

    // cout<<"new size="<<str->size()<<" x0="<<x0<<" y0="<<y0<<endl;
    letter.focalPoint->resize(0);
    for(int i = 0; i < str.size(); i++) {
      str[i].x -= PICT_SIZE / 2;
      str[i].y -= PICT_SIZE / 2;
      letter.focalPoint->push_back(str[i]);
      cout << "x=" << letter.fPoint[i].x << " y=" << letter.fPoint[i].y << " p=" << (int)letter.fPoint[i].type << endl;
    }
    // exit(0);
  }
}

};