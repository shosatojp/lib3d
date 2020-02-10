#include "lib3d.h"
/**
 * フラグメントシェーダ
 */
void l3FragmentShader(l3PixelInfo* p, l3Mat31 v) {
    l3Poligon* _poligon = p->_poligon;

    switch (_poligon->material) {
        case l3PoligonMaterialVertex: {
            l3Type area_0 = l3GetTriangleArea(v, _poligon->vertices[1]->coordinate2d, _poligon->vertices[2]->coordinate2d);
            l3Type area_1 = l3GetTriangleArea(v, _poligon->vertices[2]->coordinate2d, _poligon->vertices[0]->coordinate2d);
            l3Type area_2 = l3GetTriangleArea(v, _poligon->vertices[0]->coordinate2d, _poligon->vertices[1]->coordinate2d);
            l3Type area_sum = area_0 + area_1 + area_2;
            p->color.r = area_0 / area_sum * _poligon->vertices[0]->color.r +
                         area_1 / area_sum * _poligon->vertices[1]->color.r +
                         area_2 / area_sum * _poligon->vertices[2]->color.r;
            p->color.g = area_0 / area_sum * _poligon->vertices[0]->color.g +
                         area_1 / area_sum * _poligon->vertices[1]->color.g +
                         area_2 / area_sum * _poligon->vertices[2]->color.g;
            p->color.b = area_0 / area_sum * _poligon->vertices[0]->color.b +
                         area_1 / area_sum * _poligon->vertices[1]->color.b +
                         area_2 / area_sum * _poligon->vertices[2]->color.b;
        } break;
        case l3PoligonMaterialColor: {
            memcpy(&p->color, &_poligon->color, sizeof(l3RGB));
        } break;
        case l3PoligonMaterialTexture: {
            l3Mat31A src = {0};
            l3MulMat(_poligon->textureAffineMatInv, v, src, 3, 3, 1);
            l3RGB white = {255, 255, 0};
            // l3PrintMat(_poligon->textureAffineMatInv,3,3);
            // l3PrintMat(src, 3, 1);
            memcpy(&p->color, l3GetColorAtTexture(_poligon->texture, (int)src[0], (int)src[1], &white), sizeof(l3RGB));
        } break;
    }
}

l3RGB* l3GetColorAtTexture(l3Texture* texture, int x, int y, l3RGB* _default) {
    // printf("%d %d\n", x, y);
    if (0 <= x && x < texture->w && 0 <= y && y < texture->h) {
        return (l3RGB*)&texture->buffer[(x + y * texture->w) * sizeof(unsigned char) * 3];
    } else {
        return _default;
    }
}

void l3SetTexturePoligon(l3Poligon* poligon, l3Texture* texture, l3Mat23 texture_vertices) {
    poligon->material = l3PoligonMaterialTexture;
    l3Type scale[4] = {texture->w, 0, 0, texture->h};
    poligon->textureVertices = (l3Mat23)calloc(sizeof(l3Type), 2 * 3);
    l3MulMat(scale, texture_vertices, poligon->textureVertices, 2, 2, 3);
    poligon->texture = texture;
}

void l3SetTextureMatInv(l3Poligon* poligon) {
    l3Mat33A affinemat = {0};
    l3Mat21 src[] = {&l3MatAt(poligon->textureVertices, 2, 0, 0),
                     &l3MatAt(poligon->textureVertices, 2, 0, 1),
                     &l3MatAt(poligon->textureVertices, 2, 0, 2)};
    l3Mat21 dst[] = {
        poligon->vertices[0]->coordinate2d,
        poligon->vertices[1]->coordinate2d,
        poligon->vertices[2]->coordinate2d,
    };
    l3GetAffineTransformMat33(src, dst, affinemat);
    
    l3Mat33 affinematinv = (l3Mat33)calloc(sizeof(l3Type), 3 * 3);
    l3InverseMat(3, affinemat, affinematinv);
    poligon->textureAffineMatInv = affinematinv;
}

unsigned char* l3LoadPPM(const char* path, int* w, int* h) {
    FILE* fp = fopen(path, "rb");
    if (!fp) {
        fprintf(stdout, "No such file: %s\n", path);
        return NULL;
    }

    char buf[30] = {0};

    // P6
    if (!fgets(buf, sizeof(buf), fp) || strcmp(buf, "P6\n")) {
        fprintf(stdout, "Unsupported file type (not P6): %s\n", path);
        fclose(fp);
        return NULL;
    }

    // W H
    char* endp;
    if ((endp = fgets(buf, sizeof(buf), fp)) == NULL ||
        !(*w = strtol(endp, &endp, 10)) ||
        !(*h = strtol(endp, NULL, 10))) {
        fprintf(stdout, "Unsupported file type (size): %s\n", path);
        fclose(fp);
        return NULL;
    }

    // Color
    long c;
    if (!fgets(buf, sizeof(buf), fp) ||
        !(c = strtol(buf, &endp, 10)) ||
        c != 255) {
        fprintf(stdout, "Unsupported file type (color): %s\n", path);
        fclose(fp);
        return NULL;
    }

    int size = sizeof(char) * 3 * *w * *h;
    unsigned char* buffer = (unsigned char*)malloc(size);
    if (!buffer || fread(buffer, size, 1, fp) != 1) {
        fprintf(stdout, "Unsupported file type (buffer): %s\n", path);
        fclose(fp);
        return NULL;
    }

    fclose(fp);
    return buffer;
}

int l3Load2DTexture(const char* path, l3Texture* texture) {
    if (!(texture->buffer = l3LoadPPM(path, &texture->w, &texture->h))) {
        fprintf(stdout, "Failed to load ppm file: %s\n", path);
        return -1;
    }

    return 0;
}

void l3DestructTexture(l3Texture* texture) {
    if (texture->buffer) {
        free(texture->buffer);
        texture->buffer = NULL;
    }
}