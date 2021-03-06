/*!
 *****************************************************************
 * \file
 *
 * \note
 *   Copyright (c) 2015 \n
 *   Fraunhofer Institute for Manufacturing Engineering
 *   and Automation (IPA) \n\n
 *
 *****************************************************************
 *
 * \note
 *   Project name: care-o-bot
 * \note
 *   ROS stack name: cob_control
 * \note
 *   ROS package name: cob_twist_controller
 *
 * \author
 *   Author: Marco Bezzon, email: Marco.Bezzon@ipa.fraunhofer.de
 *
 * \date Date of creation: March, 2015
 *
 * \brief
 *   This header contains the description of the unconstraint solver
 *   Implements methods from constraint_solver_base
 *
 ****************************************************************/
#ifndef GRADIENT_PROJECTION_METHOD_SOLVER_H_
#define GRADIENT_PROJECTION_METHOD_SOLVER_H_

#include <set>
#include "ros/ros.h"

#include "cob_twist_controller/cob_twist_controller_data_types.h"
#include "cob_twist_controller/constraint_solvers/solvers/constraint_solver_base.h"


#include "cob_twist_controller/constraints/constraint_base.h"
#include "cob_twist_controller/constraints/constraint.h"

class GradientProjectionMethodSolver : public ConstraintSolver<>
{
    public:
        GradientProjectionMethodSolver(InvDiffKinSolverParams &params,
                           t_Matrix6Xd &jacobian_data)
                           : ConstraintSolver(params,
                                              jacobian_data)
        {
        }

        virtual ~GradientProjectionMethodSolver()
        {
            this->clearConstraints();
        }

        /**
         * Specific implementation of solve-method to solve IK problem with constraints by using the GPM.
         * See base class ConstraintSolver for more details on params and returns.
         */
        virtual Eigen::MatrixXd solve(const t_Vector6d &in_cart_velocities,
                                      const KDL::JntArray& q,
                                      const KDL::JntArray& last_q_dot) const;

        /**
         * Set all created constraints in a (priorized) set.
         * @param constraints: All constraints ordered according to priority.
         */
        virtual void setConstraints(std::set<tConstraintBase>& constraints)
        {
            this->constraints_ = constraints;
        }

        /**
         * Calls destructor on all objects and clears the set
         */
        void clearConstraints()
        {
            this->constraints_.clear();
        }

    protected:

        /// set inserts sorted (default less operator); if element has already been added it returns an iterator on it.
        std::set<tConstraintBase> constraints_;
};

#endif /* GRADIENT_PROJECTION_METHOD_SOLVER_H_ */
