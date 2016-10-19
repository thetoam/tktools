#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cmath>
#include <sstream>
#include <string>
#include <vector>

#include "tkgeolib.h"

const long double PI = 3.141592653589793238L;

using namespace std;

string convert_kml(string name, double lat, double lon, double hgt, double twr, double tgt, vector<double> elev);
int read_horizon_file(string fileName);
int parse_location(string location, string &name, double &lat, double &lon, double &hgt, double &twr, double &tgt);
int parse_horizon(string line, vector<double> &elev);
double dms_to_decimal(string dms);
double dms_to_decimal(int d, int m, int s);

int main(int argc, char *argv[])
{

  string fileName;

  if (argc != 2)
  {
    cerr << "Usage: " << argv[0] << " <filename>" << endl;
    return -1;
  }

  fileName = string(argv[1]);
  read_horizon_file(fileName);
}

string convert_kml(string name, double lat, double lon, double hgt, double twr, double tgt, vector<double> elev)
{
  ostringstream sstream;
  string kml = "";

  kml += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
  kml += " <kml xmlns=\"http://www.opengis.net/kml/2.2\">\n";
  kml += "    <Document>\n";
  kml += "  <name>" + name + "</name>\n";
  kml += "  <open>1</open>\n";
  kml += "  <description>\n";
  kml += "    " + name + "\n";
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
  kml += "  <name>" + name + "</name>\n";
  kml += "    <styleUrl>#default0</styleUrl>\n";
  kml += "  <MultiGeometry>\n";
  kml += "    <Point>\n";
  kml += "    <coordinates>\n";

  sstream << lon << "," << lat << "," << hgt + twr;
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

  // TODO: Calculations
  for (size_t i = 0; i < elev.size(); i++)
  {
    double RE = 6371.0; //km
    double lon1, lat1;
    double A, B, C; // angles
    double a, b, c; // sides

    a = (4.0/3.0) * RE + hgt / 1000.0 + twr / 1000;
    b = (4.0/3.0) * RE + tgt / 1000.0;
    B = (90 + elev[i]) * PI / 180.0;
    A = asin(a * sin(B) / b);
    C = PI - A - B;
    c = sqrt(a * a + b * b - 2 * a * b * cos(C)); // Range (km)
    if (c > 200)
      c = 200;

    // For this azimuth, project in the direction of az (i) and a range of c (but in arc).

    for (double j = 0; j < 1; j += 0.1)
    {
      double az = i + j;
      ll_arc_distance_deg(lon, lat, c/RE, az, lon1, lat1);
      sstream << lon1 << "," << lat1 << "," << tgt;
      kml += "        " + sstream.str() + "\n";
      sstream.str(string());
    }
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


int read_horizon_file(string fileName)
{
  vector<double> elevations(360, 0.0);
  char bfr[512];
  string line;
  string siteName;
  double lat, lon, hgt, twr, tgt;
  ifstream ifs(fileName.c_str());

  // Check that this looks like a Radio Mobile horizon file:
  ifs.getline(bfr, 512);
  line = string(bfr);

  if (line.find("Observe from") != 0)
  {
    cerr << "Unexpected file format" << endl;
    return -1;
  }
  ifs.seekg(0);

  // Read the file
  ifs.getline(bfr, 512); // Observe from ... (header line)
  ifs.getline(bfr, 512); // [Site name, lat, lon, height, tower height, target height]
  line = string(bfr);


  parse_location(line, siteName, lat, lon, hgt, twr, tgt);
  //cerr << siteName << endl;
  //cerr << lat << endl;
  //cerr << lon << endl;
  //cerr << hgt << endl;
  //cerr << twr << endl;
  //cerr << tgt << endl;

  ifs.getline(bfr, 512); // Azimuth Elevation angle etc (header line)
  while (getline(ifs, line))
    parse_horizon(line, elevations);

  fileName = fileName + ".kml";
  string kml = convert_kml(siteName, lat, lon, hgt, twr, tgt, elevations);
  ofstream f(fileName.c_str());
  f << kml;
  f.close();


  return 0;
}


int parse_location(string line, string &name, double &lat, double &lon, double &hgt, double &twr, double &tgt)
{
  // 0x09 is tab character
  size_t i, j = -1;
  i = line.find((char) 0x09);
  name = line.substr(j + 1, i - j - 1);

  j = i;
  i = line.find((char) 0x09, i + 1);
  lat = dms_to_decimal(line.substr(j + 1, i - j));

  j = i;
  i = line.find((char) 0x09, i + 1);
  lon = dms_to_decimal(line.substr(j + 1, i - j));

  j = i;
  i = line.find((char) 0x09, i + 1);
  hgt = atof(line.substr(j + 1, i - j).c_str());

  j = i;
  i = line.find((char) 0x09, i + 1);
  twr = atof(line.substr(j + 1, i - j).c_str());

  j = i;
  i = line.find((char) 0x09, i + 1);
  tgt = atof(line.substr(j + 1, i - j).c_str());

  return 0;
}

int parse_horizon(string line, vector<double> &elev)
{
  int az;
  double el;
  size_t i, j = -1;
  i = line.find((char) 0x09);
  az = atoi(line.substr(j + 1, i - j).c_str());

  j = i;
  i = line.find((char) 0x09, i + 1);
  el = atof(line.substr(j + 1, i - j).c_str());

  if (el > 0.0)
    elev[az] = el;

  return 0;
}

double dms_to_decimal(string dms)
{
  string dir;
  int d, m, s;
  size_t i, j;
  i = dms.find((char) 0xB0); // Degree sign
  d = atof(dms.substr(0, i).c_str());
  j = i + 1;
  i = dms.find((char) '\'', i);
  m = atof(dms.substr(j, i - j).c_str());
  j = i + 1;
  i = dms.find((char) '\"', i);
  s = atof(dms.substr(j, i - j).c_str());
  dir = dms.substr(i + 1, 1);
  if (dir == "S" or dir == "W")
    d *= -1;
  return dms_to_decimal(d, m, s);
}

double dms_to_decimal(int d, int m, int  s)
{
  int sgn = 1;
  if (d < 0)
    sgn = -1;
  d *= sgn; // Absolute value
  return sgn * (d + m / 60.0 + s / 3600.0);
}
