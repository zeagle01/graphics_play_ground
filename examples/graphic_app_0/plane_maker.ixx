module;

#include <vector>

export module  app:plane_maker;

export
class plane_maker
{
public:
	void make_plane(
		std::vector< int>& indices,
		std::vector< int>& m_edges,
		std::vector<float>& pos,
		std::vector<float>& pos_2d,
		float lx, float ly, int nx, int ny)
	{

		float dx = lx / nx;
		float dy = ly / ny;

		pos.clear();
		pos.reserve(nx * ny);
		pos_2d.clear();
		pos_2d.reserve(nx * ny);

		for (int xi = 0; xi < nx; xi++)
		{
			for (int yi = 0; yi < ny; yi++)
			{
				pos.push_back(xi * dx);
				pos.push_back(yi * dy);
				pos.push_back(0);

				pos_2d.push_back(xi * dx);
				pos_2d.push_back(yi * dy);
			}
		}
		pos.shrink_to_fit();
		pos_2d.shrink_to_fit();

		indices.clear();
		indices.reserve((nx - 1) * (ny - 1) * 2);
		m_edges.clear();
		m_edges.reserve(nx * ny * 3);

		for (int xi = 0; xi < nx - 1; xi++)
		{
			for (int yi = 0; yi < ny - 1; yi++)
			{
				int v0 = yi + ny * xi;
				int v1 = yi + 1 + ny * xi;

				int v2 = yi + ny * (xi + 1);
				int v3 = yi + 1 + ny * (xi + 1);
				indices.push_back(v0);
				indices.push_back(v1);
				indices.push_back(v2);

				indices.push_back(v2);
				indices.push_back(v1);
				indices.push_back(v3);

				//edges
				m_edges.push_back(v0);
				m_edges.push_back(v1);

				m_edges.push_back(v0);
				m_edges.push_back(v2);

				m_edges.push_back(v1);
				m_edges.push_back(v2);

				m_edges.push_back(v2);
				m_edges.push_back(v3);

				m_edges.push_back(v1);
				m_edges.push_back(v3);
			}
		}
		indices.shrink_to_fit();
		m_edges.shrink_to_fit();
	}

private:

};
