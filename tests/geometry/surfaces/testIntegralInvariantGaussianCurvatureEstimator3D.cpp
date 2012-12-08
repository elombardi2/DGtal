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
 * @file testIntegralInvariantGaussianCurvatureEstimator3D.cpp
 * @ingroup Tests
 * @author Jérémy Levallois (\c jeremy.levallois@liris.cnrs.fr )
 * Laboratoire d'InfoRmatique en Image et Systèmes d'information - LIRIS (CNRS, UMR 5205), Université de Lyon, France
 *
 * @date 2012/11/28
 *
 * Functions for testing class IntegralInvariantGaussianCurvatureEstimator3D.
 *
 * This file is part of the DGtal library.
 */

///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "DGtal/base/Common.h"

#include "DGtal/shapes/GaussDigitizer.h"
#include "DGtal/topology/LightImplicitDigitalSurface.h"
#include "DGtal/topology/DigitalSurface.h"
#include "DGtal/geometry/surfaces/FunctorOnCells.h"
#include "DGtal/topology/DepthFirstVisitor.h"
#include "DGtal/geometry/surfaces/estimation/IntegralInvariantGaussianCurvatureEstimator.h"
#include "DGtal/math/MPolynomial.h"
#include "DGtal/io/readers/MPolynomialReader.h"
#include "DGtal/shapes/implicit/ImplicitPolynomial3Shape.h"

///////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace DGtal;

///////////////////////////////////////////////////////////////////////////////
// Functions for testing class IntegralInvariantGaussianCurvatureEstimator3D.
///////////////////////////////////////////////////////////////////////////////
/**
 * Example of a test. To be completed.
 *
 */
bool testIntegralInvariantGaussianCurvatureEstimator3D( double h, double delta )
{
  typedef Z3i::KSpace::Surfel Surfel;
  typedef Z3i::Space::RealPoint::Coordinate Ring;
  typedef MPolynomial< 3, Ring > Polynomial3;
  typedef MPolynomialReader< 3, Ring > Polynomial3Reader;
  typedef ImplicitPolynomial3Shape< Z3i::Space > MyShape;
  typedef GaussDigitizer< Z3i::Space, MyShape > MyGaussDigitizer;
  typedef LightImplicitDigitalSurface< Z3i::KSpace, MyGaussDigitizer > MyLightImplicitDigitalSurface;
  typedef DigitalSurface< MyLightImplicitDigitalSurface > MyDigitalSurface;
  typedef FunctorOnCells< MyGaussDigitizer, Z3i::KSpace > MyFunctor;
  typedef DepthFirstVisitor< MyDigitalSurface > Visitor;
  typedef typename Visitor::VertexConstIterator SurfelConstIterator;
  typedef IntegralInvariantGaussianCurvatureEstimator< Z3i::KSpace, MyFunctor > MyIIGaussianEstimator;
  typedef typename MyIIGaussianEstimator::Quantity Quantity;
  typedef typename MyShape::RealPoint RealPoint;

  string poly_str = "x^2 + y^2 + z^2 - 25";
  double border_min[3] = { -10, -10, -10 };
  double border_max[3] = { 10, 10, 10 };
  double k = 5.00153;
  double realValue = 0.04; // = 1/r^2

  trace.beginBlock ( "Testing integral invariant 3D Gaussian curvature initialization ..." );

  Polynomial3 poly;
  Polynomial3Reader reader;
  std::string::const_iterator iter = reader.read ( poly, poly_str.begin(), poly_str.end() );
  if ( iter != poly_str.end() )
  {
    std::cerr << "ERROR: I read only <"
              << poly_str.substr( 0, iter - poly_str.begin() )
              << ">, and I built P=" << poly << std::endl;
    return 1;
  }

  MyShape shape( poly );

  MyGaussDigitizer gaussDigShape;
  gaussDigShape.attach( shape );
  gaussDigShape.init( RealPoint( border_min ), RealPoint( border_max ), h );
  Z3i::Domain domain = gaussDigShape.getDomain();
  Z3i::KSpace kSpace;
  bool space_ok = kSpace.init( domain.lowerBound(), domain.upperBound(), true );
  if (!space_ok)
  {
    trace.error() << "Error in the Khalimsky space construction."<<std::endl;
    return 2;
  }

  SurfelAdjacency< Z3i::KSpace::dimension > SAdj( true );
  Surfel bel = Surfaces< Z3i::KSpace >::findABel( kSpace, gaussDigShape, 100000 );
  MyLightImplicitDigitalSurface lightImplDigSurf( kSpace, gaussDigShape, SAdj, bel );
  MyDigitalSurface digSurfShape( lightImplDigSurf );

  MyFunctor functorShape ( gaussDigShape, kSpace, true );
  MyIIGaussianEstimator estimator ( kSpace, functorShape );

  try
  {
    estimator.init( h, k );
  }
  catch(...)
  {
    trace.endBlock();
    return false;
  }

  std::vector< Quantity > resultsIICurvature;
  back_insert_iterator< std::vector< Quantity > > resultsIICurvatureIterator( resultsIICurvature );

  Visitor *depth = new Visitor( digSurfShape, *digSurfShape.begin() );
  SurfelConstIterator abegin = SurfelConstIterator( depth );
  SurfelConstIterator aend = SurfelConstIterator( 0 );

  trace.endBlock();
  trace.beginBlock ( "Testing integral invariant 3D Gaussian curvature computation ..." );

  try
  {
    estimator.eval( abegin, aend, resultsIICurvatureIterator );
  }
  catch(...)
  {
    trace.endBlock();
    return false;
  }

  trace.endBlock();

  trace.beginBlock ( "Comparing results of integral invariant 3D Gaussian curvature ..." );

  double mean = 0.0;
  unsigned int rsize = resultsIICurvature.size();

  for ( unsigned int i = 0; i < rsize; ++i )
  {
    mean += resultsIICurvature[ i ];
  }
  mean /= rsize;

  if ( std::abs ( realValue - mean ) > delta )
  {
    trace.endBlock();
    return false;
  }

  trace.endBlock();
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

int main( int argc, char** argv )
{
  trace.beginBlock ( "Testing class IntegralInvariantGaussianCurvatureEstimator3D" );
  trace.info() << "Args:";
  for ( int i = 0; i < argc; ++i )
    trace.info() << " " << argv[ i ];
  trace.info() << endl;

  bool res = testIntegralInvariantGaussianCurvatureEstimator3D( 0.6, 0.007 ); // && ... other tests
  trace.emphase() << ( res ? "Passed." : "Error." ) << endl;
  trace.endBlock();
  return res ? 0 : 1;
}
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
