#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "tkargparse.h"
#include "tkgeolib.h"


using namespace std;

void idealPolygon(vector<double> point, vector<vector<double> > &polygon);
string getKml(vector<double> point, vector<vector<double> > polygon);



int main(int argc, char *argv[])
{

  bool ideal = false;
  double lat = -999.9;
  double lon = -999.9;

  vector<double> point(3,-999.9);  // Coordinates of the centre point
  vector<vector<double> > polygon(3601, vector<double>(3, -999.9));  // Coordinates of polygon

  Options opt;
  opt.add("ideal", 'x', &ideal);
  opt.add("lat", 'd', &lat);
  opt.add("lon", 'd', &lon);
  opt.process(1, argc, argv);




  // Populate the position vector
  if (lat == -999.9 || lon == -999.9)
  {
    // If I don't have a latitude/longitude, I can't do much
    cerr << "Error: invaid or unspecified location" << endl;
    return 1;
  }
  point[0] = lon;
  point[1] = lat;
  point[2] = 0;


  // Populate the polygon vector
  if (ideal)
  {
    idealPolygon(point, polygon);
  }



  cout << getKml(point, polygon) << endl;

  return 0;
}


void idealPolygon(vector<double> point, vector<vector<double> > &polygon)
{
  const double RE = 6371.0;
  unsigned int i;
  double lon, lat;
  double az;

  for (i = 0; i < polygon.size(); i++)
  {
    az = i / 10.0;
    ll_arc_distance_deg(point[0], point[1], 200.0 / RE, az, lon, lat);
    polygon[i][0] = lon;
    polygon[i][1] = lat;
    polygon[i][2] = 0;
  }

}


string getKml(vector<double> point, vector<vector<double> > polygon)
{
  unsigned int i;
  ostringstream sstream;
  string kml = "";

  kml += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
  kml += " <kml xmlns=\"http://www.opengis.net/kml/2.2\">\n";
  kml += "    <Document>\n";
  kml += "      <name>%NAME%</name>\n";
  kml += "  <open>1</open>\n";
  kml += "  <description>\n";
  kml += "    %NAME%\n";
  kml += "  </description>\n";
  kml += "  <Style id=\"default\">\n";
  kml += "    <IconStyle>\n";
  kml += "      <color>ff0000ff</color>\n";
  kml += "      <scale>1.2</scale>\n";
  kml += "      <Icon>\n";
  kml += "        <href>http://maps.google.com/mapfiles/kml/shapes/placemark_circle.png</href>\n";
  kml += "      </Icon>\n";
  kml += "    </IconStyle>\n";
  kml += "    <LineStyle>\n";
  //kml += "      <color>7f00ffff</color>\n";
  kml += "      <width>5</width>\n";
  kml += "    </LineStyle>\n";
  kml += "    <PolyStyle>\n";
  kml += "      <fill>0</fill>\n";
  kml += "    </PolyStyle>\n";
  kml += "  </Style>\n";
  kml += "  <Style id=\"h1\">\n";
  kml += "    <IconStyle>\n";
  kml += "      <color>ff0000ff</color>\n";
  kml += "      <scale>1.2</scale>\n";
  kml += "      <Icon>\n";
  kml += "        <href>http://maps.google.com/mapfiles/kml/shapes/placemark_circle_highlight.png</href>\n";
  kml += "      </Icon>\n";
  kml += "    </IconStyle>\n";
  kml += "    <LineStyle>\n";
  kml += "      <width>5</width>\n";
  kml += "    </LineStyle>\n";
  kml += "    <PolyStyle>\n";
  kml += "      <fill>0</fill>\n";
  kml += "    </PolyStyle>\n";
  kml += "  </Style>\n";
  kml += "  <StyleMap id=\"default0\">\n";
  kml += "    <Pair>\n";
  kml += "      <key>normal</key>\n";
  kml += "      <styleUrl>#default</styleUrl>\n";
  kml += "    </Pair>\n";
  kml += "    <Pair>\n";
  kml += "      <key>highlight</key>\n";
  kml += "      <styleUrl>#h1</styleUrl>\n";
  kml += "    </Pair>\n";
  kml += "  </StyleMap>\n";
  kml += "  <Placemark>\n";
  kml += "  <name>%NAME%</name>\n";
  kml += "    <styleUrl>#default0</styleUrl>\n";
  kml += "  <MultiGeometry>\n";
  kml += "    <Point>\n";
  kml += "    <coordinates>\n";

  sstream << point[0] << "," << point[1] << "," << point[2];
  kml += "      " + sstream.str() + "\n";
  sstream.str(string());

  kml += "    </coordinates>\n";
  kml += "    </Point>\n";
  kml += "    <Polygon>\n";
  kml += "      <tessellate>1</tessellate>\n";
  kml += "      <altitudeMode>absolute</altitudeMode>\n";
  kml += "      <outerBoundaryIs>";
  kml += "      <LinearRing>";
  kml += "      <coordinates>\n";

  for (i = 0; i < polygon.size(); i++)
  {
    sstream << polygon[i][0] << "," << polygon[i][1] << "," << polygon[i][2];
    kml += "        " + sstream.str() + "\n";
    sstream.str(string());
  }


  kml += "        </coordinates>\n";
  kml += "        </LinearRing>\n";
  kml += "        </outerBoundaryIs>\n";
  kml += "      </Polygon>\n";
  kml += "    </MultiGeometry>\n";
  kml += "    </Placemark>\n";
  kml += "   </Document>\n";
  kml += " </kml>\n";

  return kml;
}



