#include <string>
#include <iostream>
#include <limits>
using namespace std;

class BarcodeImage;

class BarcodeIO
{
public:
   void virtual scan(const BarcodeImage & bc) = 0;
   void virtual readText(const string & text = "") = 0;
   void virtual generateImagefromText() = 0;
   void virtual displayImageToConsole() = 0;
   void virtual displayTextToConsole() = 0;
   void virtual translateImageToText() = 0;
};

class BarcodeImage
{
private:
   bool **imageData;
   void allocate();
   void deallocate();
   bool checkSize(const string strData[], int height);

public:
   static const int MAX_HEIGHT = 35;
   static const int MAX_WIDTH = 65;

   BarcodeImage();
   ~BarcodeImage();
   BarcodeImage(const BarcodeImage &bci);
   BarcodeImage(const string strData[], int height);
   const BarcodeImage &operator=(const BarcodeImage &rhs);
   bool getPixel(int row, int col);
   bool setPixel(int row, int col, bool value);
   void display();
};

class DataMatrix: public BarcodeIO
{
private:
   BarcodeImage image;
   string text;
   int actualWidth, actualHeight;
   int computeSignalWidth();
   int computeSignalHeight();
   bool writeChartoCol(int row, int bit);

public:
   static const char BLACK_CHAR = '*';
   static const char WHITE_CHAR = ' ';
   static const int MAX_STRING_LENGTH = 50;

   void scan(const BarcodeImage & bc);
   void readText(const string & text = "");
   void generateImagefromText();
   void displayImageToConsole();
   void displayTextToConsole(); 
   void translateImageToText();
   
   DataMatrix(const string & text = "");
   DataMatrix(const BarcodeImage & image);
   int getactualWidth() {return this->actualWidth;}
   int getactualHeight() {return this->actualHeight;}
};

int main()
{
   string s_image_in[13] = 
   { 
      "                                                      ",
      "                                                      ",
      "                                                      ",
      "* * * * * * * * * * * * * * * * * * * * * * * * * * * ",
      "*                                                    *",
      "**********   *** *********** * ****** **** *********  ",
      "* *********** ****************************************",
      "*     * *    * *   *     *     *    *          * *    ",
      "*  *      *  **  ** * * *         ***  ***  * * * ** *",
      "* ***   ***   **  * ********    * **   ***  ***  * *  ",
      "**   **   *   *         *      *      ***    ***     *",
      "*** *  * *   *** **  ***  *  *  **  * ***  * ** *  ** ",
      "******************************************************"
   };

   string s_image_in_2[12] =  
   { 
      "                                      ",
      "                                      ",
      "* * * * * * * * * * * * * * * * * * * ",
      "*                                    *",
      "**** * * ******** ** ****** *** ****  ",
      "* ******************* **********    **",
      "*    *** *      * *  *   *  *   *  ** ",
      "* *  *     * *     *   **    *      **",
      "** *   *  **** *  **  ***** * * *   * ",
      "*        *    * *  * *  **        ****",
      "* *  * *  **** *   *  *** *   *  * ** ",
      "**************************************"
   };

   BarcodeImage bc(s_image_in, 13);
   DataMatrix dm(bc);

   dm.translateImageToText();
   dm.displayTextToConsole();
   dm.displayImageToConsole();
   
   bc = BarcodeImage(s_image_in_2, 12);
   dm.scan(bc);
   dm.translateImageToText();
   dm.displayTextToConsole();
   dm.displayImageToConsole();

   dm.readText("CIS 15B rocks more than Zeppelin");
   dm.generateImagefromText();
   dm.displayTextToConsole();
   dm.displayImageToConsole();
   
   return 0;
}

BarcodeImage::BarcodeImage()
{
   imageData = NULL;
   allocate();
}

BarcodeImage::~BarcodeImage()
{
   deallocate();
}

BarcodeImage::BarcodeImage(const string strData[], int height)
{
   bool value;
   imageData = NULL;
   allocate();

   if(!checkSize(strData, height))
      return;

   for(int k = 1; k <= height; k++)
      for(int j = 0; j < strData[height-k].length(); j++)
      {
         if((strData[height-k])[j] == '*')
            value = true;
         else
            value = false;
         setPixel(MAX_HEIGHT-k, j, value);
      }
}

BarcodeImage::BarcodeImage(const BarcodeImage &bci)
{
   imageData = NULL;
   allocate();
   *this = bci;
}

const BarcodeImage &BarcodeImage::operator=(const BarcodeImage &rhs)
{
   if (this != &rhs)
   {
      for(int row = 0; row < MAX_HEIGHT; row++)
         for(int col = 0; col < MAX_WIDTH; col++)
            imageData[row][col] = rhs.imageData[row][col];
   }
   return *this;
}

bool BarcodeImage::checkSize(const string strData[], int height)
{
   if(height >= MAX_HEIGHT || height < 0)
      return false;

   for(int k = 0; k < height; k++)
      if(strData[k].length() >= MAX_WIDTH || strData[k].length() < 0)
         return false;

   return true;
}

bool BarcodeImage::setPixel(int row, int col, bool value)
{
   if (row < 0 || row >= MAX_HEIGHT || col < 0 || col >= MAX_WIDTH)
      return false;
   imageData[row][col] = value;
   return true;
}

bool BarcodeImage::getPixel(int row, int col)
{
   if (row < 0 || row >= MAX_HEIGHT || col < 0 || col >= MAX_WIDTH)
      return false;
   return imageData[row][col];
}

void BarcodeImage::display()
{
   int row, col;

   cout << endl;
   for (col = 0; col < MAX_WIDTH + 2; col++)
      cout << "-";
   cout << endl;

   for(row = 0; row < MAX_HEIGHT; row++)
   {
      cout << "|";
      for (col = 0; col < MAX_WIDTH; col++)
      {
         if (getPixel(row ,col))
            cout << "*";
         else
            cout << " ";
      }
      cout << "|" << endl;
   }

   for (col = 0; col < MAX_WIDTH + 2; col++)
      cout << "-";
   cout << endl;
}

void BarcodeImage::allocate()
{
   int row, col;

   if (imageData != NULL)
      deallocate();

   imageData = new bool*[MAX_HEIGHT];
   for (row = 0; row < MAX_HEIGHT; row++)
      imageData[row] = new bool[MAX_WIDTH];
   
   for(row = 0; row < MAX_HEIGHT; row++)
      for(col = 0; col < MAX_WIDTH; col++)
         imageData[row][col] = false;
}

void BarcodeImage::deallocate()
{
   if (imageData == NULL)
      return;

   for(int row = 0; row < MAX_HEIGHT; row++)
      delete[] imageData[row];
   delete[] imageData;
   imageData = NULL;
}

DataMatrix::DataMatrix(const string & text)
{
   readText(text);
}

DataMatrix::DataMatrix(const BarcodeImage &bc)
{
   scan(bc);
}

void DataMatrix::scan(const BarcodeImage &bc)
{
   image = bc;
   text = "";
   actualWidth = computeSignalWidth();
   actualHeight = computeSignalHeight();   
}
   
int DataMatrix::computeSignalWidth()
{
   int returnVal = 0;

   for(int col = 0; col < image.MAX_WIDTH; col++)
      if(image.getPixel(image.MAX_HEIGHT-1, col) == true)
         returnVal++;

   return returnVal;
}

int DataMatrix::computeSignalHeight()
{
   int returnVal = 0;

   for(int row = 0; row < image.MAX_HEIGHT; row++)
      if(image.getPixel(row, 0) == true)
         returnVal++;

   return returnVal;
}

void DataMatrix::readText(const string &text)
{
   BarcodeImage clear;

   image = clear;
   actualWidth = 0;
   actualHeight = 0;
   if(text.length() < 1 || text.length() >= MAX_STRING_LENGTH)
      return;
   else
      this->text = text;
}
   
void DataMatrix::generateImagefromText()
{
   int height = 0, bitVal = 0;
   actualHeight = 10;
   actualWidth = 2;

   for(int k = 0; k < actualHeight; k++)
      image.setPixel(image.MAX_HEIGHT-1-k, 0, true);

   for(int col = 1; col <= text.length(); col++)
   {
      actualWidth++;
      image.setPixel(image.MAX_HEIGHT - 1, col, true);
      image.setPixel(image.MAX_HEIGHT - actualHeight, (2*col), true);
      for(int k = 0; k < 8; k++)
      {
         height = (image.MAX_HEIGHT-2) - k;
         bitVal = (int)text[col-1];
         image.setPixel(height, col, writeChartoCol(k, bitVal));
      }
   }

   for(int j = 0; j < 5; j++)
      image.setPixel(image.MAX_HEIGHT-1-(2*j), actualWidth-1, true);
}

bool DataMatrix::writeChartoCol(int row, int bit)
{
   int bitComp, bitResult;
   bitComp = 1 << row;
   bitResult = bitComp & bit;
   return bitResult;
}

void DataMatrix::displayImageToConsole()
{
   int row = image.MAX_HEIGHT - actualHeight, col;

   cout << endl;
   for (col = 0; col < actualWidth + 2; col++)
      cout << "-";
   cout << endl;

   for(;row <= image.MAX_HEIGHT - 1; row++)
   {
      cout << "|";
      for (col = 0; col < actualWidth; col++)
      {
         if (image.getPixel(row, col) == true)
            cout << BLACK_CHAR;
         else
            cout << WHITE_CHAR;
      }
      cout << "|" << endl;
   }

   for (col = 0; col < actualWidth + 2; col++)
      cout << "-";
   cout << endl;
}

void DataMatrix::translateImageToText()
{
   int asciiInt, height, size;
   char asciiChar;
   text.resize(actualWidth-2, ' ');
      
   for(int col = 1; col < actualWidth-1; col++)
   {
      asciiInt = 0;
      for(int k = 0; k < 8; k++)
      {
         height = (image.MAX_HEIGHT-2) - k;
         if(image.getPixel(height, col))
            asciiInt += (1 << k);
      }
      asciiChar = asciiInt;
      text[col-1] = asciiChar;
   }
}

void DataMatrix::displayTextToConsole()
{
   cout << text << endl;
}
            

   