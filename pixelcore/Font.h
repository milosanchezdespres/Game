#pragma once

#include <string>
#include <cstring>
#include <vector>

#include "Display.h"

//#include "Colors.h"//TO DO : edit pixels of the image to implement color into font
#include "TextureManager.h"

//font assets must follow the same order of letters/signs as font.png
enum class LetterID
{
    a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,
    zero, one, two, three, four, five, six, seven, height, nine, 
    space, unknown//always last
};

namespace px
{
    inline LetterID char_to_enum(char c)
    {
        if (c >= 'A' && c <= 'Z') c = static_cast<char>(std::tolower(c));
        if (c >= 'a' && c <= 'z') return static_cast<LetterID>(c - 'a');
        if (c >= '0' && c <= '9') return static_cast<LetterID>(static_cast<int>(LetterID::zero) + (c - '0'));
        if (c == ' ') return LetterID::space;
        return LetterID::unknown;
    }

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

            void write(const char* text, float x, float y, int size)
            {
                int text_size = std::strlen(text);

                int space = 8 * size;
                int letter_spacing = 3 * size;

                int offsetX = 0;

                for(int i = 0; i < text_size; i++)
                {
                    char c = text[i];

                    if(std::isalpha(c) || std::isdigit(c) || c == ' ')
                    {
                        LetterID ID = char_to_enum(c);

                        if(ID != LetterID::unknown && ID != LetterID::space)
                            blit(ID, x + offsetX, y, size);
                    }

                    offsetX += space + letter_spacing;
                }
            }
    };
};