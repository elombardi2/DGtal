/**
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

/**
 * @file testFMM.cpp
 * @ingroup Tests
 * @author Tristan Roussillon (\c tristan.roussillon@liris.cnrs.fr ) Laboratoire d'InfoRmatique en
 * Image et Systèmes d'information - LIRIS (CNRS, UMR 5205), CNRS, France
 * @date 2012/01/16
 *
 * @brief Functions for the fast marching method.
 *
 * This file is part of the DGtal library.
 */

///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <iomanip>
#include "DGtal/base/Common.h"

#include "DGtal/kernel/SpaceND.h"
#include "DGtal/kernel/domains/HyperRectDomain.h"
#include "DGtal/kernel/domains/DomainPredicate.h"
#include "DGtal/kernel/PointVector.h"
#include "DGtal/kernel/BasicPointPredicates.h"

//FMM
#include "DGtal/geometry/volumes/distance/IncrementalMetricComputers.h"
#include "DGtal/geometry/volumes/distance/FMM.h"

//Display
#include "DGtal/io/colormaps/GradientColorMap.h"
#include "DGtal/io/boards/Board2D.h"

/*
#include "DGtal/shapes/Shapes.h"
#include "DGtal/helpers/StdDefs.h"
#include "DGtal/shapes/ShapeFactory.h"
*/
///////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace DGtal;

template< typename TIterator >
void draw( const TIterator& itb, const TIterator& ite, std::string basename) 
{
  typedef typename std::iterator_traits<TIterator>::value_type Pair; 
  typedef typename Pair::first_type Point; 
  typedef typename Pair::second_type Value; 
  GradientColorMap<Value, DGtal::CMAP_GRAYSCALE> colorMap(0,10); 

  Board2D b; 
  b.setUnit ( LibBoard::Board::UCentimeter );

  TIterator it = itb; 
  for ( ; it != ite; ++it)
    {
      Point p = it->first;
      b << CustomStyle( p.className(), new CustomFillColor( colorMap( it->second) ) );
      b << p;
    }

  std::stringstream s; 
  s << basename << ".eps"; 
  b.saveEPS(s.str().c_str());
} 

/**
 * Example of a test. To be completed.
 *
 */
bool testFMM()
{
  unsigned int nbok = 0;
  unsigned int nb = 0;

  typedef PointVector<2,int> Point; 
  typedef double Distance;  
  typedef PointVector<2,Distance> Distances;  
  typedef IncrementalEuclideanMetricComputer<2> MetricComputer; 

  trace.beginBlock ( "Testing metric computer " );
 
  MetricComputer m; 
  trace.info() << m.compute( Distances(0,m.infinity()) ) << std::endl; 
  trace.info() << m.compute( Distances(1,m.infinity()) ) << std::endl; 
  trace.info() << m.compute( Distances(1,1) ) << std::endl; 
  trace.info() << m.compute( Distances(std::sqrt(2),2) ) << std::endl; 

  trace.endBlock();

  trace.beginBlock ( "Testing FMM " );
 
  std::map<Point, Distance> map; 
  map.insert( std::pair<Point, Distance>( Point(0,0), 0.0 ) );
 
  FMM<MetricComputer, TruePointPredicate<Point> > f(map, m); 

  trace.info() << f << std::endl; 

  trace.endBlock();


  return nbok == nb;
}

/**
 * Simple 2d distance transform
 *
 */
bool testDisplay2dDT()
{

  static const DGtal::Dimension dimension = 2; 

  //type definitions 
  typedef HyperRectDomain< SpaceND<dimension, int> > Domain; 
  typedef Domain::Point Point;
 
  typedef IncrementalEuclideanMetricComputer<dimension> MetricComputer; 
  typedef MetricComputer::Distance Distance;  

  typedef FMM<MetricComputer, DomainPredicate<Domain> > FMM; 

  //init
  Point c(0,0); 
  Point up(5, 5); 
  Point low(-5,-5); 

  std::map<Point, Distance> map; 
  map.insert( std::pair<Point, Distance>( c, 0.0 ) );

  MetricComputer mc; 
  Domain d(low, up); 
  DomainPredicate<Domain> dp(d);

  //computation
  trace.beginBlock ( "Testing FMM " );
 
  FMM fmm(map, mc, dp); 
  fmm.compute(); 
  trace.info() << fmm << std::endl; 

  trace.endBlock();

  //display
  draw(map.begin(), map.end(), "DTbyFMM");
}








///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

int main ( int argc, char** argv )
{
  trace.beginBlock ( "Testing FMM" );
  trace.info() << "Args:";
  for ( int i = 0; i < argc; ++i )
    trace.info() << " " << argv[ i ];
  trace.info() << endl;

  bool res =  testFMM()
    && testDisplay2dDT()
;
  //&& ... other tests
  trace.emphase() << ( res ? "Passed." : "Error." ) << endl;
  trace.endBlock();
  return res ? 0 : 1;
}
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
