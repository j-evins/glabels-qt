//  Barcode2dBase.hpp
//
//  Copyright (C) 2013  Jaye Evins <evins@snaught.com>
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

#ifndef glbarcode_Barcode2dBase_hpp
#define glbarcode_Barcode2dBase_hpp


#include <string>
#include <vector>

#include "Barcode.hpp"
#include "Renderer.hpp"
#include "Matrix.hpp"


namespace glbarcode
{

        /**
         * @class Barcode2dBase Barcode2dBase.h glbarcode/Barcode2dBase.h
         *
         * The Barcode2dBase class is the base class for all 2D barcode implementations.
         * This class provides a common framework for the implementation of 2D barcodes.
         * Creating 2D barcode types (or symbologies) would be typically accomplished by
         * implementing this class rather than directly implementing the Barcode class.
         *
         */
        class Barcode2dBase : public Barcode
        {

        protected:
                /**
                 * Default constructor.
                 */
                Barcode2dBase();


        public:
                /**
                 * Destructor.
                 */
                ~Barcode2dBase() override;


                /**
                 * Build 2D barcode from data.
                 *
                 * Implements glbarcode::Barcode::build().  Calls the validate(), preprocess(),
                 * encode(), and vectorize() virtual methods, as shown:
                 *
                 * @dotfile figure-2d-build.dot "2D build() data flow"
                 *
                 * @param[in] data Data to encode in barcode
                 * @param[in] w    Requested width of barcode (0 = auto size)
                 * @param[in] h    Requested height of barcode (0 = auto size)
                 *
                 * @returns A reference to this Barcode object for chaining methods
                 */
                Barcode& build( const std::string&    data,
                                double                w = 0,
                                double                h = 0 ) override;


        protected:
                /**
                 * Validate barcode data.
                 *
                 * Required virtual method to test if data is valid for encoding with
                 * barcode type.
                 *
                 * @param[in] rawData Data to validate
                 *
                 * @return True if data is valid data for barcode type
                 * @return False if data is not valid data for barcode type
                 */
                virtual bool validate( const std::string& rawData ) = 0;


                /**
                 * Preprocess barcode data.
                 *
                 * Optional virtual method to perform any transformation of the data needed
                 * before encoding.  (E.g. encoding an extended alphabet into a simpler one).
                 *
                 * @param[in] rawData Data to preprocess
                 *
                 * @return Preprocessed data
                 */
                virtual std::string preprocess( const std::string& rawData );


                /**
                 * Encode barcode data
                 *
                 * Required virtual method to encode data such that it can be later vectorized.
                 *
                 * @param[in]  cookedData  Data to encode
                 * @param[out] encodedData Encoded data in the form of a matrix
                 *
                 * @return True if data was encoded successfully
                 * @return False if data could not be encoded (condition not discoverable by validate())
                 */
                virtual bool encode( const std::string&   cookedData,
                                     Matrix<bool>&        encodedData ) = 0;


                /**
                 * Vectorize encoded data
                 *
                 * Optional virtual method to convert encoded data into a list of drawing
                 * primitives which can later be rendered.
                 *
                 * @param[in]     encodedData Data to vectorize
                 * @param[in,out] w           Requested width of barcode (0 = auto size), vectorize will overwrite with actual width
                 * @param[in,out] h           Requested height of barcode (0 = auto size), vectorize will overwrite with actual width
                 */
                virtual void vectorize( const Matrix<bool>& encodedData,
                                        double&             w,
                                        double&             h );


        private:
                /**
                 * Barcode2dBase Private data
                 */
                struct PrivateData;
                PrivateData *d;

        };

}


#endif // glbarcode_Barcode2dBase_hpp
