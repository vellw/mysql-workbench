/*
 * Copyright (c) 2014, Oracle and/or its affiliates. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; version 2 of the
 * License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301  USA
 */

#ifndef SPATIAL_HANDLER_H_
#define SPATIAL_HANDLER_H_

#include <gdal/ogrsf_frmts.h>
#include <gdal/ogr_api.h>
#include <gdal/gdal_pam.h>
#include <gdal/memdataset.h>
#include <gdal/gdal_alg.h>
#include <deque>
#include "base/geometry.h"

namespace GIS
{
enum ProjectionType
{
  ProjMercator = 1, ProjEquirectangular = 2, ProjRobinson = 3
};
enum ShapeType
{
  ShapeUnknown, ShapePoint, ShapeLineString, ShapeLinearRing, ShapePolygon
};

struct ShapeContainer
{
  ShapeType type;
  std::vector<base::Point> points;
};

struct ProjectionView
{
  int width;
  int height;
  double MaxLat;
  double MaxLon;
  double MinLat;
  double MinLon;
  ProjectionType type;
};

class SpatialHandler
{
  OGRGeometry *poGeometry;
  std::string robinson_projection;
  std::string mercator_projection;
  std::string equirectangular_projection;
  std::string geodetic_wkt;
  double _adfProjection[6];
  double _invProjection[6];
  OGRCoordinateTransformation *geo_to_proj;
  OGRCoordinateTransformation *proj_to_geo;

private:

protected:
  void setupMatrix(ProjectionView &view);
  void extractPoints(OGRGeometry *shape,
      std::deque<ShapeContainer> &shapes_container, ProjectionType &projection);
  void convertPoints(std::vector<double> &x, std::vector<double> &y,
      ProjectionType &projection);
  ShapeContainer convertToShapeContainer(ShapeType type, std::vector<double> &x,
      std::vector<double> &y);
  char* getProjectionWkt(ProjectionType p);
public:
  SpatialHandler();
  int importFromMySQL(const std::string &data);
  virtual ~SpatialHandler();
  int getOutput(ProjectionView &view,
      std::deque<ShapeContainer> &shapes_container);
  void toLatLng(int x, int y, double &lat,
      double &lon);
  void fromLatLng(double lat, double lon, double &x,
      double &y);
};

}
#endif /* SPATIAL_HANDLER_H_ */
