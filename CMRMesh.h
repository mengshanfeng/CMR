#ifndef CMR_MESH_H
#define CMR_MESH_H

#include <assert.h>

template <class T> class CMRMesh2D;

template <class T>
class CMRMesh2DAccessor
{
	public:
		CMRMesh2DAccessor(unsigned int width,unsigned int height,T * cells);
		T & getLeft(void);
	private:
		unsigned int x;
		unsigned int y;
		unsigned int width;
		unsigned int height;
		T * cells;

	friend class CMRMesh2D<T>;
};

template <class T>
class CMRMesh2D
{
	public:
		CMRMesh2D(unsigned int width,unsigned int height,unsigned int ghost);
		virtual ~CMRMesh2D(void);
		unsigned int getWidth(void) const;
		unsigned int getHeight(void) const;
		void apply(void (*func)(T & cell_out,const T & cell_in,CMRMesh2DAccessor<T> & accessor));
		void apply(void (*func)(T & cell,CMRMesh2DAccessor<T> & accessor));
	private:
		CMRMesh2D(const CMRMesh2D<T> & orig);
		CMRMesh2D<T> & operator = (const CMRMesh2D<T> & orig);
	private:
		unsigned int width;
		unsigned int height;
		unsigned int ghost;
		T * cells1;
		T * cells2;
		T * currentCells;
};

template <class T>
CMRMesh2D<T>::CMRMesh2D(unsigned int width,unsigned int height,unsigned int ghost)
{
	//errors
	assert(ghost < width);
	assert(ghost < height);

	//update size
	width  += ghost;
	height += ghost;

	//allocate memory
	this->cells1 = new T[width * height];
	this->cells2 = new T[width * height];
	this->currentCells = cells1;

	//update local values
	this->width  = width;
	this->height = height;
	this->ghost  = ghost;
}

template <class T>
CMRMesh2D<T>::~CMRMesh2D(void)
{
	//free memory
	delete this->cells1;
	delete this->cells2;
}

template <class T>
void CMRMesh2D<T>::apply(void (*func)(T & cell_out,const T & cell_in,CMRMesh2DAccessor<T> & accessor))
{
	//vars
	T * in;
	T * out;
	T * local_in;
	T * local_out;

	//select int out
	in = currentCells;
	if (currentCells == cells1)
		out = cells2;
	else
		out = cells1;

	//loop
	for (unsigned int y = ghost ; y < height - ghost ; y++)
	{
		local_in = in + y * width;
		local_out = out + y * width;
		for (unsigned int x = ghost ; x < width - ghost ; x++)
		{
			func(local_out[x],local_in[x]);
		}
	}

	//at then end permite current
	if (currentCells == cells1)
		currentCells = cells2;
	else
		currentCells = cells1;
}

template <class T>
void CMRMesh2D<T>::apply(void (*func)(T & cell,CMRMesh2DAccessor<T> & accessor))
{
	//vars
	T * c;
	CMRMesh2DAccessor<T> accessor(width,height,currentCells);

	//loop
	for (accessor.y = ghost ; accessor.y < height - ghost ; accessor.y++)
	{
		c = currentCells + accessor.y * width;
		for (accessor.x = ghost ; accessor.x < width - ghost ; accessor.x++)
		{
			func(c[accessor.x],accessor);
		}
	}
}

template <class T>
CMRMesh2DAccessor<T>::CMRMesh2DAccessor(unsigned int width,unsigned int height,T * cells)
{
	this->width = width;
	this->height = height;
	this->cells = cells;
}

template <class T>
T & CMRMesh2DAccessor<T>::getLeft(void)
{
	return cells[y * width + x - 1];
}

#endif //CMR_MESH_H
