All scripts for FontForge project.

Most important scripts:

getAnchor.pe  - get all anchor points coordinates from base glyphs and write to text file 

_namelist_uni1.liga  - all glyphs names need to build

@liga_uni.php - _namelist_glyph_uni.pe FontForge script wich build empty glyphs and OpenType liga features for all letters in _namelist_uni1.liga 

@liga_glyph.php  - PHP script which actually build all 10 000 glyps from _namelist_uni1.liga.

@liga_glyph_init.php  - PHP script included in  @liga_glyph.php

@liga_glyph_setNexLetter.php   - PHP script included in  @liga_glyph.php

_namelist_uni_PDF.pe 
All glyphs named in for uni(code1)...(codeN) form. In this glyph names convention text with whis font can be export to PDF and copy from it with full Unicode support for ligatures.
For rename all glyps in this Unicode convention use script _namelist_uni_PDF.pe 


Any question how use scripts for this or your font?
Feel free contact me 
gomde@mail.ru
www.buddism.ru
Stroganov Alexander
