/***************************************************************************
 *   This file is part of "std_utils" library.                             *
 *   Copyright (C) 2005 by V01D                                            *
 *                                                                         *
 *   "std_utils" is free software; you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   "std_utils" is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <sstream>
#include <iostream>
#include "std_util.h"
#include "php2stl.h"

using std::string;
using std::ostringstream;
using std::istringstream;

/**
 * strips a string from a specific leading and/or trailing character.
 * \param src the string to strip.
 * \param c the character to find and remove from src.
 * \param sides indicates which "side to strip (leading, trailing or both).
 * \returns a copy of the stripped string
 */
string std_util::strip(const string& src, char c, STRIP_SIDE sides) {
 if(!src.size())return "";
  return strip(src, string(1, c), sides);
}

/**
 * strips a string from specific leading and/or trailing characters.
 * \param src the string to strip.
 * \param chars the characters to find and remove from src.
 * \param sides indicates which "side to strip (leading, trailing or both).
 * \returns a copy of the stripped string
 */
string std_util::strip(const string& src, const string& chars, STRIP_SIDE sides) {
  if(!src.size())return "";
  size_t start = 0, end = 0, length = src.length();
  if (length == 0 || chars.empty()) return string();
  
  if (sides == LEFT || sides == BOTH) start = src.find_first_not_of(chars);
  if (sides == RIGHT || sides == BOTH) end = src.find_last_not_of(chars);

  if (sides == LEFT) return (start == string::npos ? string() : src.substr(start));
  else if (sides == RIGHT) return (end == string::npos ? string() : src.substr(0, end+1));
  else {
    if (start == string::npos || end == string::npos) return string();
    else return src.substr(start, end - start + 1);
  }
}

/**
 * tokenizes a given string and returns the requested token(s) (reentrant version).
 * \param src is the string to tokenize.
 * \param dest will contain the token(s) found if the search was succesfull.
 * \param toknum is the token number to return (counting from 1).
 * \param all_tokens indicates whether gettok() should return the toknum'th token (false), or all
 * tokens including and following the toknum'th token (true).
 * \param sep indicates the character to interpret as token delimiter.
 * \returns true if the tokens requested were found.
 */
bool std_util::gettok(const std::string& src, std::string& dest, size_t toknum, bool all_tokens, char sep) {
  if (toknum < 1) return false;
  size_t length = src.size();
  if (length == 0) return false;

  size_t start = 0, tok_len = 0;
  size_t tok_count = 0;
  bool inToken = false;
  // string out;

  for (size_t i = 0; i < length; i++) {
    if (inToken) {
      if (src[i] == sep || i == (length-1)) {
        inToken = false;
        if (tok_count == toknum || (all_tokens && tok_count >= toknum)) {
          if (src[i] != sep && i == (length-1)) tok_len++;
          if (all_tokens) {
            if (tok_len != 0 && (tok_count-toknum) != 0) dest += sep;
            dest += src.substr(start, tok_len);
          }
          else { dest = src.substr(start,tok_len); return true; }
        }
      }
      if (src[i] != sep) tok_len++;
    }
    else if (src[i] != sep) { start = i; inToken = true; tok_len = 1; tok_count++; }
  }

  // If I get here it means that the last token wasn't "closed"
  if (!all_tokens && toknum == tok_count) { dest = src.substr(start, tok_len); return true; }
  else if (all_tokens && tok_count >= toknum) {
    // Getting here with all_tokens == true, doesn't really mean that I left the last tok "open", this checks it
    if (inToken) { if (tok_count != toknum) dest += sep; dest += src.substr(start, tok_len); }
    return true;
  }
  return false;
}

/**
 * tokenizes a given string and returns the requested token(s).
 * \param src is the string to tokenize.
 * \param toknum is the token number to return (counting from 1).
 * \param all_tokens indicates whether gettok() should return the toknum'th token (false), or all
 * tokens including and following the toknum'th token (true).
 * \param sep indicates the character to interpret as token delimiter.
 * \returns a copy of the token(s) found (if any), or an empty string otherwise.
 */
string std_util::gettok(const string& src, size_t toknum, bool all_tokens, char sep) {
  string out;
  if (!gettok(src, out, toknum, all_tokens, sep)) return string();
  else return out;
}

string std_util::toupper_UTF(string &src){
    if(src=="")return src;  //cout<<"input="<<src<<endl;
    string dest=src;
    dest=str_replace("й","Й",dest);
    dest=str_replace("ц","Ц",dest);
    dest=str_replace("у","У",dest);
    dest=str_replace("к","К",dest);
    dest=str_replace("е","Е",dest);
    dest=str_replace("н","Н",dest);
    dest=str_replace("г","Г",dest);
    dest=str_replace("ш","Ш",dest);
    dest=str_replace("щ","Щ",dest);
    dest=str_replace("з","З",dest);
    dest=str_replace("х","Х",dest);
    dest=str_replace("ъ","Ъ",dest);
    dest=str_replace("ф","Ф",dest);
    dest=str_replace("ы","Ы",dest);
    dest=str_replace("в","В",dest);
    dest=str_replace("а","А",dest);
    dest=str_replace("п","П",dest);
    dest=str_replace("р","Р",dest);
    dest=str_replace("о","О",dest);
    dest=str_replace("л","Л",dest);
    dest=str_replace("д","Д",dest);
    dest=str_replace("ж","Ж",dest);
    dest=str_replace("э","Э",dest);
    dest=str_replace("ё","Ё",dest);
    dest=str_replace("я","Я",dest);
    dest=str_replace("ч","Ч",dest);
    dest=str_replace("с","С",dest);
    dest=str_replace("м","М",dest);
    dest=str_replace("и","И",dest);
    dest=str_replace("т","Т",dest);
    dest=str_replace("ь","Ь",dest);
    dest=str_replace("б","Б",dest);
    dest=str_replace("ю","Ю",dest);
    
    dest=str_replace("q","q",dest);
    dest=str_replace("w","W",dest);
    dest=str_replace("e","E",dest);
    dest=str_replace("r","R",dest);
    dest=str_replace("t","T",dest);
    dest=str_replace("y","Y",dest);
    dest=str_replace("u","U",dest);
    dest=str_replace("i","I",dest);
    dest=str_replace("o","O",dest);
    dest=str_replace("p","P",dest);
    dest=str_replace("a","A",dest);
    dest=str_replace("s","S",dest);
    dest=str_replace("d","D",dest);
    dest=str_replace("f","F",dest);
    dest=str_replace("g","G",dest);
    dest=str_replace("h","H",dest);
    dest=str_replace("j","J",dest);
    dest=str_replace("k","K",dest);
    dest=str_replace("l","L",dest);
    dest=str_replace("z","Z",dest);
    dest=str_replace("x","X",dest);
    dest=str_replace("c","C",dest);
    dest=str_replace("v","V",dest);
    dest=str_replace("b","B",dest);
    dest=str_replace("n","N",dest);
    dest=str_replace("m","M",dest); 
        
    return dest;
}
string std_util::tolower_UTF(string &src){
    if(src=="")return src;
    string dest=src;
    dest=str_replace("Й","й",dest);
    dest=str_replace("Ц","ц",dest);
    dest=str_replace("У","у",dest);
    dest=str_replace("К","к",dest);
    dest=str_replace("Е","е",dest);
    dest=str_replace("Н","н",dest);
    dest=str_replace("Г","г",dest);
    dest=str_replace("Ш","ш",dest);
    dest=str_replace("Щ","щ",dest);
    dest=str_replace("З","з",dest);
    dest=str_replace("Х","х",dest);
    dest=str_replace("Ъ","ъ",dest);
    dest=str_replace("Ф","ф",dest);
    dest=str_replace("Ы","ы",dest);
    dest=str_replace("В","в",dest);
    dest=str_replace("А","а",dest);
    dest=str_replace("П","п",dest);
    dest=str_replace("Р","р",dest);
    dest=str_replace("О","о",dest);
    dest=str_replace("Л","л",dest);
    dest=str_replace("Д","д",dest);
    dest=str_replace("Ж","ж",dest);
    dest=str_replace("Э","э",dest);
    dest=str_replace("Ё","ё",dest);
    dest=str_replace("Я","я",dest);
    dest=str_replace("Ч","ч",dest);
    dest=str_replace("С","с",dest);
    dest=str_replace("М","м",dest);
    dest=str_replace("И","и",dest);
    dest=str_replace("Т","т",dest);
    dest=str_replace("Ь","ь",dest);
    dest=str_replace("Б","б",dest);
    dest=str_replace("Ю","ю",dest);
    
    dest=str_replace("q","q",dest);
    dest=str_replace("W","w",dest);
    dest=str_replace("E","e",dest);
    dest=str_replace("R","r",dest);
    dest=str_replace("T","t",dest);
    dest=str_replace("Y","y",dest);
    dest=str_replace("U","u",dest);
    dest=str_replace("I","i",dest);
    dest=str_replace("O","o",dest);
    dest=str_replace("P","p",dest);
    dest=str_replace("A","a",dest);
    dest=str_replace("S","s",dest);
    dest=str_replace("D","d",dest);
    dest=str_replace("F","f",dest);
    dest=str_replace("G","g",dest);
    dest=str_replace("H","h",dest);
    dest=str_replace("J","j",dest);
    dest=str_replace("K","k",dest);
    dest=str_replace("L","l",dest);
    dest=str_replace("Z","z",dest);
    dest=str_replace("X","x",dest);
    dest=str_replace("C","c",dest);
    dest=str_replace("V","v",dest);
    dest=str_replace("B","b",dest);
    dest=str_replace("N","n",dest);
    dest=str_replace("M","m",dest); 
        
    return dest;
    
}
string std_util::cleanSentence(string &src){
    if(src=="")return src;
    string dest=src;
    
    dest=str_replace(".","",dest);
    dest=str_replace(",","",dest);
    dest=str_replace("!","",dest);
    
    return dest;
}