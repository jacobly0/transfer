#include <ft2build.h>
#include FT_FREETYPE_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <uchar.h>

int main(int argc, char **argv) {
    FT_Library library;
    FT_Error error;
    FT_Face face;
    if ((error = FT_Init_FreeType(&library)) ||
        (error = FT_New_Face(library, "/usr/share/fonts/fonts-master/apache/robotomono/RobotoMono-Medium.ttf", 0, &face)) ||
        (error = FT_Set_Char_Size(face, 0, 550, 0, 141)) ||
        (error = FT_Load_Char(face, U'w', FT_LOAD_DEFAULT))) return error;
    int width = face->glyph->advance.x >> 6, height = face->size->metrics.height >> 6, height_bytes = (height + 1) >> 1;
    char path[PATH_MAX];
    strcpy(path, argv[0]);
    strcpy(strrchr(path, '/'), "/../src/font.h");
    FILE *header = fopen(path, "w");
    strcpy(path, argv[0]);
    strcpy(strrchr(path, '/'), "/../src/font.c");
    FILE *source = fopen(path, "w");
    fprintf(header,
            "#ifndef FONT_H\n"
            "#define FONT_H\n"
            "\n"
            "#include <stdint.h>\n"
            "\n"
            "enum {\n"
            "\tFONT_WIDTH = %d,\n"
            "\tFONT_HEIGHT = %d,\n"
            "\tFONT_HEIGHT_BYTES = (FONT_HEIGHT + 1) >> 1,\n"
            "};\n"
            "\n"
            "extern const uint8_t font[0x100][FONT_WIDTH][FONT_HEIGHT_BYTES];\n"
            "\n"
            "#endif\n",
            width, height);
    fprintf(source,
            "#include \"font.h\"\n"
            "\n"
            "const uint8_t font[0x100][FONT_WIDTH][FONT_HEIGHT_BYTES] = {\n");
    uint8_t *bitmap = calloc(width, height_bytes);
    if (!bitmap) return 1;
    for (char32_t c = U'\0'; c <= U'\xFF'; ++c) {
        memset(bitmap, 0, width * height_bytes);
        if ((error = FT_Load_Char(face, c, FT_LOAD_DEFAULT)) ||
            (error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL))) return error;
        fprintf(source, "%c{ // '", c ? ' ' : '\t');
        if (c >= U' ' && c <= U'~')
            fprintf(source, "%c", (char)c);
        else
            fprintf(source, "\\x%02hhX", (unsigned char)c);
        fprintf(source, "'\n");
        for (int sy = 0, dy = (face->size->metrics.ascender >> 6) - face->glyph->bitmap_top; sy != face->glyph->bitmap.rows; ++sy, ++dy) {
            for (int sx = 0, dx = face->glyph->bitmap_left; sx != face->glyph->bitmap.width; ++sx, ++dx) {
                if (dx < 0 || dx >= width || dy < 0 || dy >= height) continue;
                uint8_t g = face->glyph->bitmap.buffer[sy * face->glyph->bitmap.pitch + sx];
                bitmap[dx * height_bytes + (dy >> 1)] |= (g >> 4) << ((dy & 1) << 2);
            }
        }
        uint8_t *p = bitmap;
        for (int x = 0; x != width; ++x) {
            fprintf(source, "\t\t{ ");
            for (int y = 0; y != height_bytes; ++y) {
                if (y) fprintf(source, ", ");
                fprintf(source, "0x%02hhX", (unsigned char)~*p++);
            }
            fprintf(source, " },\n");
        }
        fprintf(source, "\t},");
    }
    fprintf(source, "\n};\n");
    free(bitmap);
    fclose(source);
    fclose(header);
}
