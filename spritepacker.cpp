/**
SpritePacker

Given a list of NxM images, pack them into one or more 1024x1024 images ("spritesheets"). 
The algorithm should create the fewest number of spritesheets possible.

Given a list of image sizes in stdin in the following format:

864x480 78x107 410x321 188x167 315x274 229x163 629x236 39x32 193x56 543x155

You should produce on stdout like the following. Each image line is the dimensions of the image, 
and then the X and Y coordinate of where it should be placed in the spritesheet:

sheet 1
864x480 0 0
410x321 0 480
315x274 410 480
188x167 725 480
229x163 0 801
543x155 229 801
78x107 913 0
193x56 725 647
39x32 913 107

sheet 2
629x236 0 0

Author : Piyush Verma
Email : pxv8270@rit.edu
**/

#include <cstdlib>
#include <iostream>
#include <vector>
#include <cstdio>

using namespace std;


//data structure to store image from input &
//sprite in the spritesheet
struct image{
    int width, height, x, y;
    //constructor to create an image object
    image(int width, int height){
        this->width = width;
        this->height = height;
        this->x = 0;
        this->y = 0;
    }

    //constructor to create a sprite object
    image(int width, int height, int x, int y){
        this->width = width;
        this->height = height;
        this->x = x;
        this->y = y;
    }

    //copy constructor required for sorting
    image(const image& i){
        this->width = i.width;
        this->height = i.height;
        this->x = i.x;
        this->y = i.y;
    }
};

//class to create a spritesheet to fit as many sprites
//into it as possible.

class SpriteSheet{
private:

    //private variables and lists
    int width, height;
    vector<int> widthList;
    vector<int> heightList;
    vector<bool> occupiedList;
    vector<image*> sprites;

public:

    //constructor
    SpriteSheet(int width, int height){
        this->width = width;
        this->height = height;
        widthList.push_back(width);
        heightList.push_back(height);
        occupiedList.resize(1);
        occupiedList[0] = false; //occupied list is empty
    }

    //function to return the list of sprites stored in the spritesheet
    vector<image*> getSprites(){return sprites;}

    //function to insert as maximum amount of images in the spritesheet
    void insertImages(vector<image*> &images){
        int i = 0;
        while(i < images.size()){
            if(insertImage(*images[i])){
                images.erase(images.begin() + i);
            }else{
                i++;
            }
        }
    }


    //function that returns if a particular area in the
    //spritesheet is occupied or not.
    bool isOccupied(int i, int j){
        int occupiedIndex = i * widthList.size() + j;
        if(occupiedIndex < occupiedList.size())
            return occupiedList[occupiedIndex];

        return false;
    }


    //function to insert a single image as a sprite in the spritesheet
    //at the most appropriate location
    bool insertImage(const image& img){
        int x = 0, y = 0;
        //outer loop
        for(int i = 0; i<heightList.size(); i++){
            x = 0;
            //if there is a big enough area in the spritesheet
            if(heightList[i] >= img.height){
                int occupiedWidth = 0;
                for(int j = 0; j<widthList.size(); j++){
                    //if occupied, then skip the area
                    if(isOccupied(i, j))
                        occupiedWidth += widthList[j];
                    //otherwise area found, insert the image in that area
                    else{
                        int remainingWidth = width - occupiedWidth;
                        if(remainingWidth >= img.width){
                            image* sprite = new image(img.width, img.height, x, y);
                            sprites.push_back(sprite);

                            updateImageArea(i, j, img.width, img.height);
                            return true;
                        }
                    }
                    x += widthList[j];
                }
            }
            y += heightList[i];
        }
        return false;
    }

    //function to update the spritesheet area after inserting a new image
    void updateImageArea(int row, int column, int width, int height){

        //update the width and height lists
        updateDimension(widthList, column, width);
        updateDimension(heightList, row, height);

        int newRow = heightList.size();
        int newCol = widthList.size();

        //update the occupied list
        occupiedList.resize(newRow * newCol);

        int x = 0, y = 0, w = 0, h = 0;

        //check the areas which are occupied and update the
        //occupied list accordingly
        for(int i = 0; i<newRow; i++){
            x = 0;
            h = heightList[i];
            for(int j = 0; j < newCol; j++){
                occupiedList[i*newCol + j] = false;
                w = widthList[j];
                int spriteSize = sprites.size();
                for(int k = 0; k<spriteSize; k++){
                    image* sprite = sprites[k];
                    //check if a particular area is filled with an image sprite
                    if(!((x >= (sprite->x + sprite->width)) || (x + w < sprite->x)||
                        (y >= (sprite->y + sprite->height)) || (y + h < sprite->y))){
                            occupiedList[i*newCol + j] = true;
                            break;
                    }
                }
                x += widthList[j];
            }
            y += heightList[i];
        }
    }

    //function to update the height and width lists
    void updateDimension(vector<int>& arrList, int index, int newValue){
        int currentValue = arrList[index];

        //new value is greater than the current value, 
        //calculate the difference in value and insert it in the
        //appropriate location
        if (newValue > currentValue){
            int i = index + 1;
            int restValue = newValue - currentValue;
            while (restValue - arrList[i] > 0){
                restValue -= arrList[i];
                i++;
            }
            if (restValue != 0){
                arrList[i] -= restValue;
                arrList.insert(arrList.begin() + i, restValue);
            }
        }
        //otherwise inser the new value in the begining of the list
        else if (newValue < currentValue){
            arrList[index] -= newValue;
            arrList.insert(arrList.begin() + index, newValue);
        }
    }
};



int size;
char* input = new char[100];

void reverseBubbleSort(vector<image*> &images);


int main(int argc, char* argv[]){
    //take the input from the user or another program
    cout<<"Enter the number of images : ";
    cin>>size;
    cin.ignore();
    cout<<"Enter the list of images : ";
    vector<image*> images;
    int width, height;
    //store the image dimensions in a vector
    for(int i = 0; i<size; i++){
        cin.getline(input, 100);
        sscanf(input, "%dx%d", &width, &height);
        images.push_back(new image(width, height));
    }
    //sort the vector according to the decreasing
    //order of their heights
    reverseBubbleSort(images);

    vector<SpriteSheet> sheets;

    //add the images to spritesheets
    while(images.size() > 0){
        SpriteSheet s(1024, 1024);
        s.insertImages(images);
        sheets.push_back(s);
    }


    //display the result
    int sheetSize = sheets.size();
    for(int i = 0; i<sheetSize; i++){
        cout<<"sheet "<<i+1<<endl;
        vector<image*> sprite = sheets[i].getSprites();
        for(int j = 0; j<sprite.size(); j++){
            cout<<sprite[j]->width<<"x"<<sprite[j]->height<<" "<<sprite[j]->x<<" "<<sprite[j]->y<<endl;
        }
        cout<<endl;
    }

    
}

//function to sort the images according to their
//decreasing order of heights.
void reverseBubbleSort(vector<image*> &images){
    for(int i = 0; i<size; i++){
        for(int j = i+1; j<size; j++){
            if(images[j]->height > images[i]->height){
                image* tmp = new image(*images[i]);
                images[i] = images[j];
                images[j] = tmp;
            }
        }
    }
}

