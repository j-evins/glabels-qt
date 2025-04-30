/*  Matrix.h
 *
 *  Copyright (C) 2013  Jaye Evins <evins@snaught.com>
 *
 *  This file is part of glbarcode++.
 *
 *  glbarcode++ is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  glbarcode++ is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with glbarcode++.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef glbarcode_Matrix_h
#define glbarcode_Matrix_h


namespace glbarcode
{

	/**
	 * @class Matrix Matrix.h glbarcode/Matrix.h
	 *
	 * Simple 2D Matrix implementation
	 */
	template <class T> class Matrix
	{
		
	public:
		/**
		 * Default constructor.
		 */
		Matrix() : mNx(0), mNy(0), mData(nullptr) { }


		/**
		 * Sized constructor.
		 */
		Matrix( int nx, int ny ) : mNx(nx),
			                   mNy(ny),
			                   mData((nx > 0 && ny > 0) ? new T[nx * ny] : nullptr) { }


		/**
		 * Copy constructor.
		 */
		Matrix( const Matrix<T>& src ) : mNx(src.mNx),
			                         mNy(src.mNy),
			                         mData((src.mNx > 0 && src.mNy > 0) ? new T[src.mNx * src.mNy] : nullptr)
		{
			for ( int iy = 0; iy < mNy; iy++ )
			{
				for ( int ix = 0; ix < mNx; ix++ )
				{
					(*this)[iy][ix] = src[iy][ix];
				}
			}
		}


		/**
		 * Submatrix copy constructor.
		 */
		Matrix( const Matrix<T>& src,
			int              x0,
			int              y0,
			int              nx,
			int              ny ) : mNx(nx),
			                        mNy(ny),
			                        mData((nx > 0 && ny > 0) ? new T[nx * ny] : nullptr)
		{
			for ( int iy = 0; iy < mNy; iy++ )
			{
				if ( (y0+iy) < src.ny() )
				{
					for ( int ix = 0; ix < mNx; ix++ )
					{
						if ( (x0+ix) < src.nx() )
						{
							(*this)[iy][ix] = src[y0+iy][x0+ix];
						}
					}
				}
			}
		}


		/**
		 * Destructor.
		 */
		~Matrix()
		{
			if ( mData != nullptr )
			{
				delete[] mData;
			}
		}


		/**
		 * Copy assignment "=" operator
		 */
		inline Matrix & operator=( const Matrix & src )
		{
			return Matrix( src );
		}


		/**
		 * Indirection "[]" operator
		 */
		inline T* operator[]( int i ) const
		{
			return (mData + (mNx * i));
		}


		/**
		 * Resize (destroys old content)
		 */
		inline void resize( int nx, int ny )
		{
			if ( mData != nullptr )
			{
				delete[] mData;
			}
			mNx = nx;
			mNy = ny;
			mData = (nx > 0 && ny > 0) ? new T[nx * ny] : nullptr;
		}


		/**
		 * Get accessor for "nx" parameter.
		 *
		 * @returns Value of "nx" parameter
		 */
		inline int nx() const
		{
			return mNx;
		}


		/**
		 * Get accessor for "ny" parameter.
		 *
		 * @returns Value of "ny" parameter
		 */
		inline int ny() const
		{
			return mNy;
		}


		/**
		 * Extract sub-matrix from this matrix
		 */
		inline Matrix<T> subMatrix( int x0, int y0, int nx, int ny )
		{
			return Matrix<T>( *this, x0, y0, nx, ny );
		}


		/**
		 * Set sub-matrix
		 */
		inline void setSubMatrix( int x0, int y0, Matrix<T> & a )
		{
			for ( int iy = 0; iy < a.ny(); iy++ )
			{
				if ( (y0 + iy) < mNy )
				{
					for ( int ix = 0; ix < a.nx(); ix++ )
					{
						if ( (x0 + ix) < mNx )
						{
							(*this)[y0+iy][x0+ix] = a[iy][ix];
						}
					}
				}
			}
		}


		/**
		 * Fill matrix with single value
		 */
		inline void fill( T val )
		{
			for ( int iy = 0; iy < mNy; iy++ )
			{
				for ( int ix = 0; ix < mNx; ix++ )
				{
					(*this)[iy][ix] = val;
				}
			}
		}


	private:
		/**
		 * Matrix Private data
		 */
		int mNx;
		int mNy;
		T*  mData;

	};

}


#endif // glbarcode_Matrix_h
