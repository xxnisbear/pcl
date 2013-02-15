/*
 * Software License Agreement (BSD License)
 *
 * Point Cloud Library (PCL) - www.pointclouds.org
 * Copyright (c) 2010-2011, Willow Garage, Inc.
 * Copyright (c) 2012-, Open Perception, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided
 * with the distribution.
 * * Neither the name of the copyright holder(s) nor the names of its
 * contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * ground_based_people_detection_app.h
 * Created on: Nov 30, 2012
 * Author: Matteo Munaro
 */

#ifndef PCL_PEOPLE_GROUND_BASED_PEOPLE_DETECTION_APP_H_
#define PCL_PEOPLE_GROUND_BASED_PEOPLE_DETECTION_APP_H_

#include <pcl/point_types.h>
#include <pcl/sample_consensus/sac_model_plane.h>
#include <pcl/sample_consensus/ransac.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/segmentation/extract_clusters.h>
#include <pcl/kdtree/kdtree.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/people/person_cluster.h>
#include <pcl/people/head_based_subcluster.h>
#include <pcl/people/person_classifier.h>

namespace pcl
{
	namespace people
	{
		/** \brief GroundBasedPeopleDetectionApp performs people detection on RGB-D data having as input the ground plane coefficients.
		 * It implements the people detection algorithm described here:
		 * M. Munaro, F. Basso and E. Menegatti,
		 * Tracking people within groups with RGB-D data,
		 * In Proceedings of the International Conference on Intelligent Robots and Systems (IROS) 2012, Vilamoura (Portugal), 2012.
		 *
		 * \author Matteo Munaro
		 * \ingroup people
		 */
		template <typename PointT> class GroundBasedPeopleDetectionApp;

		template <typename PointT>
		class GroundBasedPeopleDetectionApp
		{
		public:

			typedef pcl::PointCloud<PointT> PointCloud;
			typedef boost::shared_ptr<PointCloud> PointCloudPtr;
			typedef boost::shared_ptr<const PointCloud> PointCloudConstPtr;

			/** \brief Constructor. */
			GroundBasedPeopleDetectionApp ();

			/** \brief Destructor. */
			virtual ~GroundBasedPeopleDetectionApp ();

			/**
			 * \brief Compute subclusters and return them into a vector of PersonCluster.
			 *
			 * \param[in] clusters the persons clusters resulting from the subclustering process.
			 */
			void
			subcluster (std::vector<pcl::people::PersonCluster<PointT> >& clusters);


			/**
			 * \brief Set the pointer to the input cloud.
			 *
			 * \param[in] cloud A pointer to the input cloud.
			 */
			void
			setInputCloud (PointCloudPtr& cloud);

			/**
			 * \brief Set the ground coefficients.
			 *
			 * \param[in] ground_coeffs Vector containing the four plane coefficients.
			 */
			void
			setGround (Eigen::VectorXf& ground_coeffs);

			/**
			 * \brief Set voxel size. Default = 0.06m.
			 *
			 * \param[in] voxel_size Value of the voxel dimension.
			 */
			void
			setVoxelSize (float voxel_size);

			/**
			 * \brief Set intrinsic parameters of the RGB camera.
			 *
			 * \param[in] intrinsics_matrix RGB camera intrinsic parameters matrix.
			 */
			void
			setIntrinsics (Eigen::Matrix3f intrinsics_matrix);

			/**
			 * \brief Set SVM-based person classifier.
			 *
			 * \param[in] person_classifier Needed for people detection on RGB data.
			 */
			void
			setClassifier (pcl::people::PersonClassifier person_classifier);

			/**
			 * \brief Set sensor orientation (vertical = true means portrait mode, vertical = false means landscape mode). Default = false.
			 *
			 * \param[in] vertical Set landscape/portait camera orientation.
			 */
			void
			setSensorPortraitOrientation (bool vertical);

			/**
			 * \brief Set head_centroid_ to true (person centroid is in the head) or false (person centroid is the whole body centroid). Default = true.
			 *
			 * \param[in] head_centroid Set the location of the person centroid (head or body center).
			 */
			void
			setHeadCentroid (bool head_centroid);

			/**
			 * \brief Set minimum and maximum allowed height for a person cluster.
			 *
			 * \param[in] min_height Minimum allowed height for a person cluster.
			 * \param[in] max_height Maximum allowed height for a person cluster.
			 */
			void
			setHeightLimits (float min_height, float max_height);

			/**
			 * \brief Set minimum and maximum allowed number of points for a person cluster.
			 *
			 * \param[in] min_points Minimum allowed number of points for a person cluster.
       * \param[in] max_points Maximum allowed number of points for a person cluster.
			 */
			void
			setDimensionLimits (int min_points, int max_points);

			/**
			 * Set minimum distance between people head. Default = 0.3 meters.
			 * @set minimum distance between people head. Default = 0.3 meters.
			 */
			void
			setMinimumDistanceBetweenHeads (float heads_minimum_distance);

			/**
			 * Get minimum and maximum allowed height for a person cluster.
			 * @get minimum and maximum allowed height for a person cluster.
			 */
			void
			getHeightLimits (float& min_height, float& max_height);

			/**
			 * Get minimum and maximum allowed number of points for a person cluster.
			 * @get minimum and maximum allowed number of points for a person cluster.
			 */
			void
			getDimensionLimits (int& min_points, int& max_points);

			/**
			 * Get minimum distance between people head. Default = 0.3 meters.
			 * @get minimum distance between people head. Default = 0.3 meters.
			 */
			float
			getMinimumDistanceBetweenHeads ();

			/**
			 * Get floor coefficients.
			 * @get floor coefficients.
			 */
			Eigen::VectorXf
			getGround ();

			/**
			 * Extract RGB information from a point cloud and output the corresponding RGB point cloud.
			 * @extract RGB information from a point cloud and output the corresponding RGB point cloud
			 */
			void
			extractRGBFromPointCloud (PointCloudPtr input_cloud, pcl::PointCloud<pcl::RGB>::Ptr& output_cloud);

			/**
			 * Swap rows/cols dimensions of a RGB point cloud (90 degrees counterclockwise rotation).
			 * @swap rows/cols dimensions of a RGB point cloud (90 degrees counterclockwise rotation).
			 */
			void
			swapDimensions (pcl::PointCloud<pcl::RGB>::Ptr& cloud);

			/**
			 * Perform people detection on the input data and return people clusters information.
			 * @perform people detection on the input data and return people clusters information.
			 */
			void
			compute (std::vector<pcl::people::PersonCluster<PointT> >& clusters);

		protected:
			float voxel_size_;									// voxel size
			Eigen::VectorXf ground_coeffs_;						// ground plane coefficients
			float sqrt_ground_coeffs_;							// ground plane normalization factor
			PointCloudPtr cloud_;								// pointer to the input cloud
			pcl::PointCloud<pcl::RGB>::Ptr rgb_image_;			// pointer to the input RGB cloud
			float max_height_;									// person clusters maximum height from the ground plane;
			float min_height_;									// person clusters minimum height from the ground plane;
			bool vertical_;										// if true, the sensor is considered to be vertically placed (portrait mode)
			bool head_centroid_;								// if true, the person centroid is computed as the centroid of the cluster points belonging to the head (default = false)
																// if false, the person centroid is computed as the centroid of the whole cluster points (default = true)
			int max_points_;									// maximum number of points for a person cluster
			int min_points_;									// minimum number of points for a person cluster
			bool dimension_limits_set_;							// true if min_points and max_points have been set by the user, false otherwise
			float heads_minimum_distance_;						// minimum distance between persons' head
			Eigen::Matrix3f intrinsics_matrix_;					// intrinsic parameters matrix of the RGB camera
			pcl::people::PersonClassifier person_classifier_;	// SVM-based person classifier
			bool person_classifier_set_flag_;				// flag stating if the classifier has been set or not
		};
	} /* namespace people */
} /* namespace pcl */
#include <pcl/people/impl/ground_based_people_detection_app.hpp>
#endif /* PCL_PEOPLE_GROUND_BASED_PEOPLE_DETECTION_APP_H_ */