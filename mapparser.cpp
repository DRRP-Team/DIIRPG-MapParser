/**
 * Copyright (c) 2019 DRRP-Team (UsernameAK)
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdio.h>
#include <time.h>
#include <cstdint>

#define DEBUG false

struct  __attribute__((packed)) map_header_t {
    uint8_t version;
    uint32_t timestamp;
    uint16_t spawnpos;
    uint8_t spawndir;
    uint8_t mapflags;
    uint8_t numsecrets;
    uint8_t numloot;
    uint16_t numnodes;
    uint16_t numnodesleaf;
    uint16_t numlinedefs;
    uint16_t numnormals;
    uint16_t numpolys;
    uint16_t numverts;
    uint16_t numspritesnormal;
    uint16_t numspritesz;
    uint16_t numtileevents;
    uint16_t bytecodesize;
    uint8_t nummayacameras;
    uint16_t nummayacamerakeys;
    uint16_t nummayatweenss[6];
};

struct __attribute__((packed)) map_normal_t {
    uint16_t x, y, z;
};

struct __attribute__((packed)) map_polygon_t {
    uint8_t texture;
    uint8_t flags;
};

struct __attribute__((packed)) map_vertex_t {
    uint8_t x, y, z, u, v;
};

int main(int argc, char **argv) {
    map_header_t header;

    FILE *f = fopen(argv[1], "rb");
    fread(&header, sizeof(header), 1, f);
    if (DEBUG) printf("Map format version %d\n", header.version);
    time_t time = header.timestamp;
    struct tm* tm_info = localtime(&time);
    char buffer[256];
    strftime(buffer, 256, "%Y-%m-%d %H:%M:%S", tm_info);
    if (DEBUG)  printf("Created at: %s\n", buffer);
    if (DEBUG)  printf("%d verts; %d polys\n", header.numverts, header.numpolys);

    uint32_t marker;

    fread(&marker, sizeof(marker), 1, f);
    if (DEBUG)  printf("marker: %x\n", marker);

    uint16_t numtexids;
    fread(&numtexids, sizeof(numtexids), 1, f);

    uint16_t texids[numtexids];
    fread(&texids, sizeof(texids), 1, f);

    fread(&marker, sizeof(marker), 1, f);
    if (DEBUG)  printf("marker: %x\n", marker);

    map_normal_t normals[header.numnormals];

    fread(&normals, sizeof(normals), 1, f);


    fread(&marker, sizeof(marker), 1, f);
    if (DEBUG) printf("marker: %x\n", marker);

    fseek(f, header.numnodes * sizeof(uint16_t), SEEK_CUR);

    fread(&marker, sizeof(marker), 1, f);
    if (DEBUG) printf("marker: %x\n", marker);

    fseek(f, header.numnodes * sizeof(uint8_t), SEEK_CUR);
    
    fread(&marker, sizeof(marker), 1, f);
    if (DEBUG) printf("marker: %x\n", marker);

    fseek(f, header.numnodes * sizeof(uint16_t), SEEK_CUR);
    fseek(f, header.numnodes * sizeof(uint16_t), SEEK_CUR);

    fread(&marker, sizeof(marker), 1, f);
    if (DEBUG) printf("marker: %x\n", marker);

    fseek(f, header.numnodes * sizeof(uint8_t) * 2, SEEK_CUR);
    fseek(f, header.numnodes * sizeof(uint8_t) * 2, SEEK_CUR);

    fread(&marker, sizeof(marker), 1, f);
    if (DEBUG) printf("marker: %x\n", marker);

    fseek(f, header.numnodesleaf * sizeof(uint16_t), SEEK_CUR);
    fseek(f, header.numnodesleaf * sizeof(uint16_t), SEEK_CUR);

    fread(&marker, sizeof(marker), 1, f);
    if (DEBUG) printf("marker: %x\n", marker);

    map_polygon_t polys[header.numpolys];

    for(int i = 0; i < header.numpolys; i++) {
        fread(&polys[i].texture, sizeof(polys[i].texture), 1, f);
    }

    for(int i = 0; i < header.numpolys; i++) {
        fread(&polys[i].flags, sizeof(polys[i].flags), 1, f);
        int numverts = (polys[i].flags & 7) + 2;
        if(numverts != 2) {
            if (DEBUG) printf("poly %d: %d verts\n", i, numverts);
        } else {
            const char *axis;
            switch(polys[i].flags & 24) {
                case 0: axis = "x"; break;
                case 8: axis = "y"; break;
                case 16: axis = "z"; break;
            }
            if (DEBUG) printf("poly %d: rect, axis %s\n", i, axis);
        }
    }

    map_vertex_t verts[header.numverts];

    for(int i = 0; i < header.numverts; i++) {
        fread(&verts[i].x, sizeof(verts[i].x), 1, f);
    }

    for(int i = 0; i < header.numverts; i++) {
        fread(&verts[i].y, sizeof(verts[i].y), 1, f);
    }

    for(int i = 0; i < header.numverts; i++) {
        fread(&verts[i].z, sizeof(verts[i].z), 1, f);
    }

    for(int i = 0; i < header.numverts; i++) {
        fread(&verts[i].u, sizeof(verts[i].u), 1, f);
    }

    for(int i = 0; i < header.numverts; i++) {
        fread(&verts[i].v, sizeof(verts[i].v), 1, f);
    }

    fread(&marker, sizeof(marker), 1, f);
    if (DEBUG)  printf("#marker: %x\n", marker);

    int vert_id = 0;
    int out_vert_id = 0;
    for(int i = 0; i < header.numpolys; i++) {
        int polyverts = (polys[i].flags & 7) + 2;
        if(polyverts != 2) {
            for(int j = 0; j < polyverts; j++) {
                printf("v %d %d %d\n", verts[vert_id].x, verts[vert_id].y, verts[vert_id].z);
                vert_id++;
                out_vert_id++;
            }
        } else {
            map_vertex_t mv[4];
            mv[0] = verts[vert_id];
            mv[2] = verts[vert_id + 1];

            mv[1].x = mv[0].x;
            mv[1].y = mv[0].y;
            mv[1].z = mv[0].z;
            mv[1].u = mv[0].u;
            mv[1].v = mv[0].v;

            mv[3].x = mv[2].x;
            mv[3].y = mv[2].y;
            mv[3].z = mv[2].z;
            mv[3].u = mv[2].u;
            mv[3].v = mv[2].v;

            switch(polys[i].flags & 24) {
            case 0:
            default:
                mv[1].x = mv[2].x;
                mv[3].x = mv[0].x;
            break;
            case 8:
                mv[1].y = mv[2].y;
                mv[3].y = mv[0].y;
            break;
            case 16:
                mv[1].z = mv[2].z;
                mv[3].z = mv[0].z;
            }
            polyverts = 4;

            for(int j = 0; j < polyverts; j++) {
                printf("v %d %d %d\n", mv[j].x, mv[j].y, mv[j].z);
                out_vert_id++;
            }

            vert_id += 2;
        }
        printf("f");
        for(int j = out_vert_id - polyverts; j < out_vert_id; j++) {
            printf(" %d", j + 1);
        }
        printf("\n");
    }

    fread(&marker, sizeof(marker), 1, f);
    if (DEBUG)  printf("marker: %x\n", marker);

    fclose(f);

    return 0;
}
