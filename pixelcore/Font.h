#pragma once

#include <string>
#include <vector>

#include "Display.h"

//#include "Colors.h"//TO DO : edit pixels of the image to implement color into font
#include "TextureManager.h"

//font assets must follow the same order of letters/signs as font.png
enum class LetterID
{
    a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,
    zero, one, two, three, four, five, six, seven, height, nine
};

namespace px
{
    struct Font
    {
        private:
            std::vector<Surface> letters;
            
            std::string alias;
            Surface& letter(LetterID ID) { return letters[static_cast<int>(ID)]; }

        public:
            Font(std::string path, Color color = 0xFFFFFFFF)
            {
                alias = path;
                load_texture(alias);
                texture(alias)->apply(color);

                int x = texture(path)->width() / 8;
                int y = texture(path)->height() / 8;

                for(int j = 0; j < y; j++) { for(int i = 0; i < x; i++)
                { letters.push_back({(float)(i * 8), (float)(j * 8), 8, 8}); } }
            }

            void blit(LetterID ID, float x, float y, int size)
                { SCREEN().blit(texture(alias), x, y, size, letter(ID)); }
    };
};