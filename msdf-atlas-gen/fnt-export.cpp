
#include "json-export.h"
#include <sstream>
#include <fstream>
namespace msdf_atlas {

static const char * imageTypeString(ImageType type) {
    switch (type) {
        case ImageType::HARD_MASK:
            return "hardmask";
        case ImageType::SOFT_MASK:
            return "softmask";
        case ImageType::SDF:
            return "sdf";
        case ImageType::PSDF:
            return "psdf";
        case ImageType::MSDF:
            return "msdf";
        case ImageType::MTSDF:
            return "mtsdf";
    }
    return nullptr;
}

bool exportFNT(const FontGeometry *fonts, int fontCount, double fontSize, double pxRange, int atlasWidth, int atlasHeight, ImageType imageType, const char *filename, const char *imageName ){
	if(fontCount>1){
		return false;
	}
	const FontGeometry &font = fonts[0];
	std::ofstream f;

	f.open(filename);
	if (!f){
		return false;
	}
	std::stringstream ss;
	ss << "info face=\"\" size="<<fontSize<< " bold=1 italic=0 charset="" unicode=1 stretchH=100 smooth=1 aa=0 padding=0,0,0,0 spacing=1,1 outline=0" << std::endl;
	ss << "common lineHeight=0 base=0 scaleW="<< atlasWidth<<" scaleH="<<atlasHeight <<" pages=1 packed=0 alphaChnl=0 redChnl=0 greenChnl=0 blueChnl=0" << std::endl;
	ss << "page id=0 file="<< imageName << std::endl;
	if (imageType == ImageType::SDF || imageType == ImageType::PSDF || imageType == ImageType::MSDF || imageType == ImageType::MTSDF){
	ss << "distanceField fieldType="<<imageTypeString(imageType) <<" distanceRange="<<pxRange << std::endl;
	}
	ss << "chars count=" << font.getGlyphs().size() << std::endl;
	for (const GlyphGeometry &glyph : font.getGlyphs()) {
		int x, y, w, h;
        glyph.getBoxRect(x, y, w, h);
		ss <<"char id="<< glyph.getCodepoint() <<"   x="<<x <<"  y="<<y <<"     width=" << w<<"     height="<< h <<"    xoffset=0   yoffset=0    xadvance=" << int(glyph.getAdvance()/font.getGeometryScale()) << "    page=0  chnl=1"<< std::endl;
	}
	int kerningCount =font.getKerning().size();
	if(kerningCount >0){
		ss<< "kernings count=" << kerningCount <<std::endl;
		for (const std::pair<std::pair<int, int>, double> &kernPair : font.getKerning()) {
			const GlyphGeometry *glyph1 = font.getGlyph(msdfgen::GlyphIndex(kernPair.first.first));
			const GlyphGeometry *glyph2 = font.getGlyph(msdfgen::GlyphIndex(kernPair.first.second));
			if (glyph1 && glyph2 && glyph1->getCodepoint() && glyph2->getCodepoint()) {
					ss <<"kerning first="<< glyph1->getCodepoint() << "  second="<< glyph2->getCodepoint()<< " amount="<< int(kernPair.second/font.getGeometryScale()) << std::endl;
			}
		}
	}
	f << ss.str();
	f.close();
	return true;
}

}
