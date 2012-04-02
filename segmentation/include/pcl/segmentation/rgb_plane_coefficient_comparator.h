/*
 * Software License Agreement (BSD License)
 *
 *  Point Cloud Library (PCL) - www.pointclouds.org
 *  Copyright (c) 2010-2012, Willow Garage, Inc.
 *
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Willow Garage, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 *
 *
 */

#ifndef PCL_RGB_SEGMENTATION_PLANE_COEFFICIENT_COMPARATOR_H_
#define PCL_RGB_SEGMENTATION_PLANE_COEFFICIENT_COMPARATOR_H_

#include <pcl/segmentation/plane_coefficient_comparator.h>
#include <boost/make_shared.hpp>

namespace pcl
{
  /** \brief RGBPlaneCoefficientComparator is a Comparator that operates on plane coefficients, 
    * for use in planar segmentation.  Also takes into account RGB, so we can segmented different colored co-planar regions.
    * In conjunction with OrganizedConnectedComponentSegmentation, this allows planes to be segmented from organized data.
    *
    * \author Alex Trevor
    */
  template<typename PointT, typename PointNT>
  class RGBPlaneCoefficientComparator: public PlaneCoefficientComparator<PointT, PointNT>
  {
    public:
      typedef typename Comparator<PointT>::PointCloud PointCloud;
      typedef typename Comparator<PointT>::PointCloudConstPtr PointCloudConstPtr;
      
      typedef typename pcl::PointCloud<PointNT> PointCloudN;
      typedef typename PointCloudN::Ptr PointCloudNPtr;
      typedef typename PointCloudN::ConstPtr PointCloudNConstPtr;
      
      using pcl::Comparator<PointT>::input_;
      using pcl::PlaneCoefficientComparator<PointT, PointNT>::normals_;
      using pcl::PlaneCoefficientComparator<PointT, PointNT>::plane_coeff_d_;

      /** \brief Empty constructor for RGBPlaneCoefficientComparator. */
      RGBPlaneCoefficientComparator ()
        : angular_threshold_ (0), distance_threshold_ (0.02), color_threshold_ (50.0)
      {
      }

      /** \brief Constructor for RGBPlaneCoefficientComparator.
        * \param[in] plane_coeff_d a reference to a vector of d coefficients of plane equations.  Must be the same size as the input cloud and input normals.  a, b, and c coefficients are in the input normals.
        */
      RGBPlaneCoefficientComparator (boost::shared_ptr<std::vector<float> >& plane_coeff_d) 
        : angular_threshold_ (0), distance_threshold_ (0.02), color_threshold_ (50.0)
      {
      }
      
      /** \brief Destructor for RGBPlaneCoefficientComparator. */
      virtual
      ~RGBPlaneCoefficientComparator ()
      {
      }

      /** \brief Provide a pointer to the input normals.
        * \param[in] normals the input normal cloud
        */
      inline void
      setInputNormals (const PointCloudNConstPtr &normals)
      {
        normals_ = normals;
      }

      /** \brief Get the input normals. */
      inline PointCloudNConstPtr
      getInputNormals () const
      {
        return (normals_);
      }

      /** \brief Provide a pointer to a vector of the d-coefficient of the planes' hessian normal form.  a, b, and c are provided by the normal cloud.
        * \param[in] plane_coeff_d a pointer to the plane coefficients.
        */
      void
      setPlaneCoeffD (boost::shared_ptr<std::vector<float> >& plane_coeff_d)
      {
        plane_coeff_d_ = plane_coeff_d;
      }

      /** \brief Provide a pointer to a vector of the d-coefficient of the planes' hessian normal form.  a, b, and c are provided by the normal cloud.
        * \param[in] plane_coeff_d a pointer to the plane coefficients.
        */
      void
      setPlaneCoeffD (std::vector<float>& plane_coeff_d)
      {
        plane_coeff_d_ = boost::make_shared<std::vector<float> >(plane_coeff_d);
      }
      
      /** \brief Get a pointer to the vector of the d-coefficient of the planes' hessian normal form. */
      const std::vector<float>&
      getPlaneCoeffD () const
      {
        return (plane_coeff_d_);
      }

      /** \brief Set the tolerance in radians for difference in normal direction between neighboring points, to be considered part of the same plane.
        * \param[in] angular_threshold the tolerance in radians
        */
      inline void
      setAngularThreshold (float angular_threshold)
      {
        angular_threshold_ = cosf (angular_threshold);
      }
      
      /** \brief Get the angular threshold in radians for difference in normal direction between neighboring points, to be considered part of the same plane. */
      inline float
      getAngularThreshold () const
      {
        return (acosf (angular_threshold_) );
      }

      /** \brief Set the tolerance in meters for difference in perpendicular distance (d component of plane equation) to the plane between neighboring points, to be considered part of the same plane.
        * \param[in] distance_threshold the tolerance in meters
        */
      inline void
      setDistanceThreshold (float distance_threshold)
      {
        distance_threshold_ = distance_threshold * distance_threshold;
      }

      /** \brief Get the distance threshold in meters (d component of plane equation) between neighboring points, to be considered part of the same plane. */
      inline float
      getDistanceThreshold () const
      {
        return (distance_threshold_);
      }

      /** \brief Set the tolerance in color space between neighboring points, to be considered part of the same plane.
        * \param[in] color_threshold The distance in color space
        */
      inline void
      setColorThreshold (float color_threshold)
      {
        color_threshold_ = color_threshold * color_threshold;
      }

      /** \brief Get the color threshold between neighboring points, to be considered part of the same plane. */
      inline float
      getColorThreshold () const
      {
        return (color_threshold_);
      }
      
      /** \brief Compare two neighboring points, by using normal information, euclidean distance, and color information.
        * \param[in] idx1 The index of the first point.
        * \param[in] idx2 The index of the second point.
        */
      bool
      compare (int idx1, int idx2) const
      {
        double dx = input_->points[idx1].x - input_->points[idx2].x;
        double dy = input_->points[idx1].y - input_->points[idx2].y;
        double dz = input_->points[idx1].z - input_->points[idx2].z;
        double dist = sqrt (dx*dx + dy*dy + dz*dz);
        int dr = input_->points[idx1].r - input_->points[idx2].r;
        int dg = input_->points[idx1].g - input_->points[idx2].g;
        int db = input_->points[idx1].b - input_->points[idx2].b;
        //Note: This is not the best metric for color comparisons, we should probably use HSV space.
        float color_dist = static_cast<float> (dr*dr + dg*dg + db*db);
        return ( (dist < distance_threshold_)
                 && (normals_->points[idx1].getNormalVector3fMap ().dot (normals_->points[idx2].getNormalVector3fMap () ) > angular_threshold_ )
                 && (color_dist < color_threshold_));
      }
      
    protected:
      float angular_threshold_;
      float distance_threshold_;
      float color_threshold_;
  };
}

#endif // PCL_SEGMENTATION_PLANE_COEFFICIENT_COMPARATOR_H_