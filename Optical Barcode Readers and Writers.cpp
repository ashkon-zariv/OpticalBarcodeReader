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
   void virtual displayTextToConsole() = 0;
   void virtual displayImageToConsole() = 0;
   void virtual generateImagefromText() = 0;
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

public:
   static const char BLACK_CHAR = '*';
   static const char WHITE_CHAR = ' ';

   DataMatrix(const string & text = "");
   DataMatrix(const BarcodeImage & image);
   readText(string text);
   scan(const BarcodeIMage & bc);
   getactualWidth() {return this->actualWidth;}
   getactualHeight() {return this->getactualHeight;}
}

int main()
{
   string sImageIn[6] = 
   { 
      "* ***** ** ** ** ****",
      " *  **** *** **** * *",
      "*** ** **** ***   ***",
      " *                 * ",
      "* ***   ***   ***  **",
      "*** * *** ****  ** **"
   };

   BarcodeImage brcd(sImageIn, 6);
   brcd.display();
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