//  BarcodeDataMatrix.cpp
//
//  Copyright (C) 2013-2026  Jaye Evins <evins@snaught.com>
//
//  This file is part of glbarcode++.
//
//  glbarcode++ is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  glbarcode++ is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with glbarcode++.  If not, see <http://www.gnu.org/licenses/>.
//


#include "BarcodeDataMatrix.hpp"

#include "Constants.hpp"

#include <cstdint>
#include <vector>
#include <cctype>
#include <algorithm>


using namespace glbarcode;
using namespace glbarcode::Constants;


namespace
{
        /* ASCII Encoding Codeword values */
        const uint8_t CW_PAD     = 129;
        const uint8_t CW_NUM_00  = 130;
        const uint8_t CW_UPSHIFT = 235;


        struct DMParameterEntry
        {
                int   nDataTotal;
                int   nXtotal;
                int   nYtotal;
                int   nBlocks1;
                int   nBlocks2;
                int   nDataBlock1;
                int   nDataBlock2;
                int   nEccBlock;
                int   aSelect;
                int   nXregions;
                int   nYregions;
                int   nXregion;
                int   nYregion;
        };

        const DMParameterEntry params[] =
        {
                {     3,  10,  10, 1, 0,   3,   0,  5,  0, 1, 1,  8,  8 },
                {     5,  12,  12, 1, 0,   5,   0,  7,  1, 1, 1, 10, 10 },
                {     8,  14,  14, 1, 0,   8,   0, 10,  2, 1, 1, 12, 12 },
                {    12,  16,  16, 1, 0,  12,   0, 12,  4, 1, 1, 14, 14 },
                {    18,  18,  18, 1, 0,  18,   0, 14,  5, 1, 1, 16, 16 },
                {    22,  20,  20, 1, 0,  22,   0, 18,  6, 1, 1, 18, 18 },
                {    30,  22,  22, 1, 0,  30,   0, 20,  7, 1, 1, 20, 20 },
                {    36,  24,  24, 1, 0,  36,   0, 24,  8, 1, 1, 22, 22 },
                {    44,  26,  26, 1, 0,  44,   0, 28,  9, 1, 1, 24, 24 },
                {    62,  32,  32, 1, 0,  62,   0, 36, 10, 2, 2, 14, 14 },
                {    86,  36,  36, 1, 0,  86,   0, 42, 11, 2, 2, 16, 16 },
                {   114,  40,  40, 1, 0, 114,   0, 48, 12, 2, 2, 18, 18 },
                {   144,  44,  44, 1, 0, 144,   0, 56, 13, 2, 2, 20, 20 },
                {   174,  48,  48, 1, 0, 174,   0, 68, 15, 2, 2, 22, 22 },
                {   204,  52,  52, 2, 0, 102,   0, 42, 11, 2, 2, 24, 24 },
                {   280,  64,  64, 2, 0, 140,   0, 56, 13, 4, 4, 14, 14 },
                {   368,  72,  72, 4, 0,  92,   0, 36, 10, 4, 4, 16, 16 },
                {   456,  80,  80, 4, 0, 114,   0, 48, 12, 4, 4, 18, 18 },
                {   576,  88,  88, 4, 0, 144,   0, 56, 13, 4, 4, 20, 20 },
                {   696,  96,  96, 4, 0, 174,   0, 68, 15, 4, 4, 22, 22 },
                {   816, 104, 104, 6, 0, 136,   0, 56, 13, 4, 4, 24, 24 },
                {  1050, 120, 120, 6, 0, 175,   0, 68, 15, 6, 6, 18, 18 },
                {  1304, 132, 132, 8, 0, 163,   0, 62, 14, 6, 6, 20, 20 },
                {  1558, 144, 144, 8, 2, 156, 155, 62, 14, 6, 6, 22, 22 },
                {  9999,   0,   0, 0, 0,   0,   0,  0,  0, 0, 0,  0,  0 } /* End of Table */
        };


        const uint8_t a[16][68] =
        {
                /* 0. Factor table for 5 RS codewords */
                {
                         62, 111,  15,  48, 228,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
                },

                /* 1. Factor table for 7 RS codewords */
                {
                        254,  92, 240, 134, 144,  68,  23,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                },

                /* 2. Factor table for 10 RS codewords */
                {
                         61, 110, 255, 116, 248, 223, 166, 185,  24,  28,   0,   0,   0,   0,   0,   0,   0,
                          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                },

                /* 3. Factor table for 11 RS codewords */
                {
                        120,  97,  60, 245,  39, 168, 194,  12, 205, 138, 175,   0,   0,   0,   0,   0,   0,
                          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                },

                /* 4. Factor table for 12 RS codewords */
                {
                        242, 100, 178,  97, 213, 142,  42,  61,  91, 158, 153,  41,   0,   0,   0,   0,   0,
                          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                },

                /* 5. Factor table for 14 RS codewords */
                {
                        185,  83, 186,  18,  45, 138, 119, 157,   9,  95, 252, 192,  97, 156,   0,   0,   0,
                          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                },

                /* 6. Factor table for 18 RS codewords */
                {
                        188,  90,  48, 225, 254,  94, 129, 109, 213, 241,  61,  66,  75, 188,  39, 100, 195,
                         83,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
                },

                /* 7. Factor table for 20 RS codewords */
                {
                        172, 186, 174,  27,  82, 108,  79, 253, 145, 153, 160, 188,   2, 168,  71, 233,   9,
                        244, 195,  15,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
                },

                /* 8. Factor table for 24 RS codewords */
                {
                        193,  50,  96, 184, 181,  12, 124, 254, 172,   5,  21, 155, 223, 251, 197, 155,  21,
                        176,  39, 109, 205,  88, 190,  52,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
                },

                /* 9. Factor table for 28 RS codewords */
                {
                        255,  93, 168, 233, 151, 120, 136, 141, 213, 110, 138,  17, 121, 249,  34,  75,  53,
                        170, 151,  37, 174, 103,  96,  71,  97,  43, 231, 211,   0,   0,   0,   0,   0,   0,
                          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
                },

                /* 10. Factor table for 36 RS codewords */
                {
                        112,  81,  98, 225,  25,  59, 184, 175,  44, 115, 119,  95, 137, 101,  33,  68,   4,
                          2,  18, 229, 182,  80, 251, 220, 179,  84, 120, 102, 181, 162, 250, 130, 218, 242,
                        127, 245,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
                },

                /* 11. Factor table for 42 RS codewords */
                {
                          5,   9,   5, 226, 177, 150,  50,  69, 202, 248, 101,  54,  57, 253,   1,  21, 121,
                         57, 111, 214, 105, 167,   9, 100,  95, 175,   8, 242, 133, 245,   2, 122, 105, 247,
                        153,  22,  38,  19,  31, 137, 193,  77,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
                },

                /* 12. Factor table for 48 RS codewords */
                {
                         19, 225, 253,  92, 213,  69, 175, 160, 147, 187,  87, 176,  44,  82, 240, 186, 138,
                         66, 100, 120,  88, 131, 205, 170,  90,  37,  23, 118, 147,  16, 106, 191,  87, 237,
                        188, 205, 231, 238, 133, 238,  22, 117,  32,  96, 223, 172, 132, 245,   0,   0,   0,
                          0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
                },

                /* 13. Factor table for 56 RS codewords */
                {
                         46, 143,  53, 233, 107, 203,  43, 155,  28, 247,  67, 127, 245, 137,  13, 164, 207,
                         62, 117, 201, 150,  22, 238, 144, 232,  29, 203, 117, 234, 218, 146, 228,  54, 132,
                        200,  38, 223,  36, 159, 150, 235, 215, 192, 230, 170, 175,  29, 100, 208, 220,  17,
                         12, 238, 223,   9, 175,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
                },

                /* 14. Factor table for 62 RS codewords */
                {
                        204,  11,  47,  86, 124, 224, 166,  94,   7, 232, 107,   4, 170, 176,  31, 163,  17,
                        188, 130,  40,  10,  87,  63,  51, 218,  27,   6, 147,  44, 161,  71, 114,  64, 175,
                        221, 185, 106, 250, 190, 197,  63, 245, 230, 134, 112, 185,  37, 196, 108, 143, 189,
                        201, 188, 202, 118,  39, 210, 144,  50, 169,  93, 242,   0,   0,   0,   0,   0,   0
                },

                /* 15. Factor table for 68 RS codewords */
                {
                        186,  82, 103,  96,  63, 132, 153, 108,  54,  64, 189, 211, 232,  49,  25, 172,  52,
                         59, 241, 181, 239, 223, 136, 231, 210,  96, 232, 220,  25, 179, 167, 202, 185, 153,
                        139,  66, 236, 227, 160,  15, 213,  93, 122,  68, 177, 158, 197, 234, 180, 248, 136,
                        213, 127,  73,  36, 154, 244, 147,  33,  89,  56, 159, 149, 251,  89, 173, 228, 220
                }
        };


        const int Log[] =
        {
                -255, 255,   1, 240,   2, 225, 241,  53,   3,  38, 226, 133, 242,  43,  54, 210,
                   4, 195,  39, 114, 227, 106, 134,  28, 243, 140,  44,  23,  55, 118, 211, 234,
                   5, 219, 196,  96,  40, 222, 115, 103, 228,  78, 107, 125, 135,   8,  29, 162,
                 244, 186, 141, 180,  45,  99,  24,  49,  56,  13, 119, 153, 212, 199, 235,  91,
                   6,  76, 220, 217, 197,  11,  97, 184,  41,  36, 223, 253, 116, 138, 104, 193,
                 229,  86,  79, 171, 108, 165, 126, 145, 136,  34,   9,  74,  30,  32, 163,  84,
                 245, 173, 187, 204, 142,  81, 181, 190,  46,  88, 100, 159,  25, 231,  50, 207,
                  57, 147,  14,  67, 120, 128, 154, 248, 213, 167, 200,  63, 236, 110,  92, 176,
                   7, 161,  77, 124, 221, 102, 218,  95, 198,  90,  12, 152,  98,  48, 185, 179,
                  42, 209,  37, 132, 224,  52, 254, 239, 117, 233, 139,  22, 105,  27, 194, 113,
                 230, 206,  87, 158,  80, 189, 172, 203, 109, 175, 166,  62, 127, 247, 146,  66,
                 137, 192,  35, 252,  10, 183,  75, 216,  31,  83,  33,  73, 164, 144,  85, 170,
                 246,  65, 174,  61, 188, 202, 205, 157, 143, 169,  82,  72, 182, 215, 191, 251,
                  47, 178,  89, 151, 101,  94, 160, 123,  26, 112, 232,  21,  51, 238, 208, 131,
                  58,  69, 148,  18,  15,  16,  68,  17, 121, 149, 129,  19, 155,  59, 249,  70,
                 214, 250, 168,  71, 201, 156,  64,  60, 237, 130, 111,  20,  93, 122, 177, 150
        };


        const int Alog[] =
        {
                  1,   2,   4,   8,  16,  32,  64, 128,  45,  90, 180,  69, 138,  57, 114, 228,
                229, 231, 227, 235, 251, 219, 155,  27,  54, 108, 216, 157,  23,  46,  92, 184,
                 93, 186,  89, 178,  73, 146,   9,  18,  36,  72, 144,  13,  26,  52, 104, 208,
                141,  55, 110, 220, 149,   7,  14,  28,  56, 112, 224, 237, 247, 195, 171, 123,
                246, 193, 175, 115, 230, 225, 239, 243, 203, 187,  91, 182,  65, 130,  41,  82,
                164, 101, 202, 185,  95, 190,  81, 162, 105, 210, 137,  63, 126, 252, 213, 135,
                 35,  70, 140,  53, 106, 212, 133,  39,  78, 156,  21,  42,  84, 168, 125, 250,
                217, 159,  19,  38,  76, 152,  29,  58, 116, 232, 253, 215, 131,  43,  86, 172,
                117, 234, 249, 223, 147,  11,  22,  44,  88, 176,  77, 154,  25,  50, 100, 200,
                189,  87, 174, 113, 226, 233, 255, 211, 139,  59, 118, 236, 245, 199, 163, 107,
                214, 129,  47,  94, 188,  85, 170, 121, 242, 201, 191,  83, 166,  97, 194, 169,
                127, 254, 209, 143,  51, 102, 204, 181,  71, 142,  49,  98, 196, 165, 103, 206,
                177,  79, 158,  17,  34,  68, 136,  61, 122, 244, 197, 167,  99, 198, 161, 111,
                222, 145,  15,  30,  60, 120, 240, 205, 183,  67, 134,  33,  66, 132,  37,  74,
                148,   5,  10,  20,  40,  80, 160, 109, 218, 153,  31,  62, 124, 248, 221, 151,
                  3,   6,  12,  24,  48,  96, 192, 173, 119, 238, 241, 207, 179,  75, 150,   1
        };


        int ecc200Encode( const std::string& data, std::vector<uint8_t>& codewords )
        {
                /*
                 * Encode data into codewords using ASCII encoding method.
                 */
                for ( unsigned int i = 0; i < data.size(); i++ )
                {
                        uint8_t c = data[i];

                        if ( c < 128 )
                        {
                                uint8_t c1 = data[i+1];

                                if ( (i < (data.size()-1)) && isdigit(c) && isdigit(c1) )
                                {
                                        /* 2-digit data 00 - 99 */
                                        codewords.push_back( CW_NUM_00 + (c-'0')*10 + (c1-'0') );
                                        i++; /* skip next input char */
                                }
                                else
                                {
                                        /* Simple ASCII data (ASCII value + 1) */
                                        codewords.push_back( c + 1 );
                                }
                        }
                        else
                        {
                                /* Extended ASCII range (128-255) */
                                codewords.push_back( CW_UPSHIFT );
                                codewords.push_back( c - 127 );
                        }
                }

                return int( codewords.size() );
        }


        const DMParameterEntry* ecc200BestSizeParams( int nRawCw )
        {

                if ( nRawCw > 1558 )
                {
                        return nullptr;
                }

                int iParam = 0;

                while ( nRawCw > params[iParam].nDataTotal )
                {
                        iParam++;
                }

                return &params[iParam];
        }


        void ecc200Fill( std::vector<uint8_t>& codewords, int nRawCw, int nDataTotal )
        {
                int nFillCw   = nDataTotal - nRawCw;

                if ( nFillCw > 0 )
                {
                        codewords.push_back( CW_PAD );
                }

                for ( int i = nRawCw + 1; i < nDataTotal; i++ )
                {
                        int r = (149*(i+1))%253 + 1;
                        codewords.push_back( (CW_PAD + r) % 254 );
                }
        }


        void ecc200EccBlock( const std::vector<uint8_t>& codewords,
                             std::vector<uint8_t>&       ecc,
                             int                         n,
                             int                         nc,
                             int                         aSelect,
                             int                         offset,
                             int                         stride )
        {
                for ( int i = 0; i < n; i++ )
                {
                        uint8_t k = ecc[offset] ^ codewords[i*stride + offset];

                        for ( int j = 0; j < (nc-1); j++ )
                        {
                                uint8_t c = a[aSelect][j];

                                if ( k != 0 )
                                {
                                        ecc[j*stride+offset] = ecc[(j+1)*stride + offset] ^ Alog[ (Log[k] + Log[c]) % 255 ];
                                }
                                else
                                {
                                        ecc[j*stride+offset] = ecc[(j+1)*stride + offset];
                                }
                        }
                        uint8_t c = a[aSelect][nc-1];
                        if ( k != 0 )
                        {
                                ecc[(nc-1)*stride + offset] = Alog[ (Log[k] + Log[c]) % 255 ];
                        }
                        else
                        {
                                ecc[(nc-1)*stride + offset] = 0;
                        }
                }
        }


        void module( Matrix<bool>& matrix,
                     Matrix<bool>& used,
                     int           ix,
                     int           iy,
                     uint8_t       codeword,
                     int           bit )
        {
                if ( iy < 0 )
                {
                        ix += 4 - ((matrix.ny()+4) % 8 );
                        iy += matrix.ny();
                }

                if ( ix < 0 )
                {
                        ix += matrix.nx();
                        iy += 4 - ((matrix.nx()+4) % 8 );
                }

                used[iy][ix] = true;

                if ( codeword & (1 << bit) )
                {
                        matrix[iy][ix] = true;
                }
        }


        void corner1( Matrix<bool>& matrix,
                      Matrix<bool>& used,
                      uint8_t       codeword )
        {
                int nx = matrix.nx();
                int ny = matrix.ny();

                module( matrix, used, 0,    ny-1, codeword, 7 );
                module( matrix, used, 1,    ny-1, codeword, 6 );
                module( matrix, used, 2,    ny-1, codeword, 5 );
                module( matrix, used, nx-2, 0,    codeword, 4 );
                module( matrix, used, nx-1, 0,    codeword, 3 );
                module( matrix, used, nx-1, 1,    codeword, 2 );
                module( matrix, used, nx-1, 2,    codeword, 1 );
                module( matrix, used, nx-1, 3,    codeword, 0 );
        }


        void corner2( Matrix<bool>& matrix,
                      Matrix<bool>& used,
                      uint8_t       codeword )
        {
                int nx = matrix.nx();
                int ny = matrix.ny();

                module( matrix, used, 0,    ny-3, codeword, 7 );
                module( matrix, used, 0,    ny-2, codeword, 6 );
                module( matrix, used, 0,    ny-1, codeword, 5 );
                module( matrix, used, nx-4, 0,    codeword, 4 );
                module( matrix, used, nx-3, 0,    codeword, 3 );
                module( matrix, used, nx-2, 0,    codeword, 2 );
                module( matrix, used, nx-1, 0,    codeword, 1 );
                module( matrix, used, nx-1, 1,    codeword, 0 );
        }


        void corner3( Matrix<bool>& matrix,
                      Matrix<bool>& used,
                      uint8_t       codeword )
        {
                int nx = matrix.nx();
                int ny = matrix.ny();

                module( matrix, used, 0,    ny-3, codeword, 7 );
                module( matrix, used, 0,    ny-2, codeword, 6 );
                module( matrix, used, 0,    ny-1, codeword, 5 );
                module( matrix, used, nx-2, 0,    codeword, 4 );
                module( matrix, used, nx-1, 0,    codeword, 3 );
                module( matrix, used, nx-1, 1,    codeword, 2 );
                module( matrix, used, nx-1, 2,    codeword, 1 );
                module( matrix, used, nx-1, 3,    codeword, 0 );
        }


        void corner4( Matrix<bool>& matrix,
                      Matrix<bool>& used,
                      uint8_t       codeword )
        {
                int nx = matrix.nx();
                int ny = matrix.ny();

                module( matrix, used, 0,    ny-1, codeword, 7 );
                module( matrix, used, nx-1, ny-1, codeword, 6 );
                module( matrix, used, nx-3, 0,    codeword, 5 );
                module( matrix, used, nx-2, 0,    codeword, 4 );
                module( matrix, used, nx-1, 0,    codeword, 3 );
                module( matrix, used, nx-3, 1,    codeword, 2 );
                module( matrix, used, nx-2, 1,    codeword, 1 );
                module( matrix, used, nx-1, 1,    codeword, 0 );
        }


        void utah( Matrix<bool>& matrix,
                   Matrix<bool>& used,
                   int           ix,
                   int           iy,
                   uint8_t       codeword )
        {
                module( matrix, used, ix-2, iy-2, codeword, 7 );
                module( matrix, used, ix-1, iy-2, codeword, 6 );
                module( matrix, used, ix-2, iy-1, codeword, 5 );
                module( matrix, used, ix-1, iy-1, codeword, 4 );
                module( matrix, used, ix,   iy-1, codeword, 3 );
                module( matrix, used, ix-2, iy,   codeword, 2 );
                module( matrix, used, ix-1, iy,   codeword, 1 );
                module( matrix, used, ix,   iy,   codeword, 0 );
        }


        void ecc200FillMatrix( Matrix<bool>&               matrix,
                               const std::vector<uint8_t>& codewords )
        {
                matrix.fill( false );

                Matrix<bool> used = matrix;

                int i  = 0;
                int ix = 0;
                int iy = 4;
                int nx = matrix.nx();
                int ny = matrix.ny();

                do {
                        if ( (iy == ny)   && (ix == 0)                ) corner1( matrix, used, codewords[i++] );
                        if ( (iy == ny-2) && (ix == 0) && (nx%4 != 0) ) corner2( matrix, used, codewords[i++] );
                        if ( (iy == ny-2) && (ix == 0) && (nx%8 == 4) ) corner3( matrix, used, codewords[i++] );
                        if ( (iy == ny+4) && (ix == 2) && (nx%8 == 0) ) corner4( matrix, used, codewords[i++] );

                        do {
                                if ( (iy < ny) && (ix >= 0) && !used[iy][ix] ) utah( matrix, used, ix, iy, codewords[i++] );
                                ix += 2;
                                iy -= 2;
                        } while ( (iy >= 0) && (ix < nx) );
                        ix += 3;
                        iy += 1;

                        do {
                                if ( (iy >= 0) && (ix < nx) && !used[iy][ix] ) utah( matrix, used, ix, iy, codewords[i++] );
                                ix -= 2;
                                iy += 2;
                        } while ( (iy < ny) && (ix >= 0) );
                        ix += 1;
                        iy += 3;

                } while  ( (iy < ny) || (ix < nx) );

                if ( !used[ny-1][nx-1] )
                {
                        matrix[ny-1][nx-1] = true;
                        matrix[ny-2][nx-2] = true;
                }
        }


        void finderPattern( Matrix<bool>& encodedData, int x0, int y0, int nx, int ny )
        {
                for ( int ix = 0; ix < nx; ix++ )
                {
                        encodedData[y0+ny-1][x0+ix] = true;
                }

                for ( int iy = 0; iy < ny; iy++ )
                {
                        encodedData[y0+iy][x0] = true;
                }

                for ( int ix = 0; ix < nx; ix += 2 )
                {
                        encodedData[y0][x0+ix]   = true;
                        encodedData[y0][x0+ix+1] = false;
                }

                for ( int iy = 0; iy < ny; iy += 2 )
                {
                        encodedData[y0+iy][x0+nx-1]   = false;
                        encodedData[y0+iy+1][x0+nx-1] = true;
                }

        }

}


namespace glbarcode
{

        /*
         * Static DataMatrix barcode creation method
         */
        Barcode* BarcodeDataMatrix::create( )
        {
                return new BarcodeDataMatrix();
        }


        /*
         * DataMatrix data validation, implements Barcode2dBase::validate()
         */
        bool BarcodeDataMatrix::validate( const std::string& rawData )
        {
                return true;
        }


        /*
         * DataMatrix data encoding, implements Barcode2dBase::encode()
         */
        bool BarcodeDataMatrix::encode( const std::string& cookedData, Matrix<bool>& encodedData )
        {
                std::vector<uint8_t> codewords;

                /*
                 * Encode data into codewords
                 */
                int nRawCw = ecc200Encode( cookedData, codewords );

                /*
                 * Determine parameters for "best size"
                 */
                const DMParameterEntry * p = ecc200BestSizeParams( nRawCw );
                if ( p == nullptr )
                {
                        return false;
                }
                encodedData.resize( p->nXtotal, p->nYtotal );


                /*
                 * Fill any extra data codewords
                 */
                ecc200Fill( codewords, nRawCw, p->nDataTotal );


                /*
                 * Calculate Reed-Solomon correction codewords
                 */
                int nTotalBlocks = p->nBlocks1 + p->nBlocks2;

                std::vector<uint8_t> ecc( p->nEccBlock*nTotalBlocks, 0 );

                for ( int iBlock = 0; iBlock < p->nBlocks1; iBlock++ )
                {
                        ecc200EccBlock( codewords, ecc, p->nDataBlock1, p->nEccBlock, p->aSelect, iBlock, nTotalBlocks );
                }

                for ( int iBlock = p->nBlocks1; iBlock < nTotalBlocks; iBlock++ )
                {
                        ecc200EccBlock( codewords, ecc, p->nDataBlock2, p->nEccBlock, p->aSelect, iBlock, nTotalBlocks );
                }

                codewords.insert( codewords.end(), ecc.begin(), ecc.end() ); /* Append to data */


                /*
                 * Create raw data matrix
                 */
                Matrix<bool> matrix( p->nXregions * p->nXregion, p->nYregions * p->nYregion );
                ecc200FillMatrix( matrix, codewords );


                /*
                 * Construct by separating out regions and inserting finder patterns
                 */
                int xstride = p->nXregion + 2;
                int ystride = p->nYregion + 2;

                for ( int iXregion = 0; iXregion < p->nXregions; iXregion++ )
                {
                        for ( int iYregion = 0; iYregion < p->nYregions; iYregion++ )
                        {
                                Matrix<bool> region = matrix.subMatrix( iXregion*p->nXregion, iYregion*p->nYregion,
                                                                        p->nXregion, p->nYregion );

                                encodedData.setSubMatrix( iXregion*xstride + 1, iYregion*ystride + 1, region );
                                finderPattern( encodedData, iXregion*xstride, iYregion*ystride, xstride, ystride );
                        }
                }


                return true;
        }


}
