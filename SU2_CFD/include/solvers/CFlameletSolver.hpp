/*!
 * \file CFlameletSolver.hpp
 * \brief Declaration and inlines for the flamelet solver class.
 * \author D. Mayer, T. Economon, N.Beishuizen, G. Tamanampudi
 * \version 6.2.0 "Falcon"
 *
 * The current SU2 release has been coordinated by the
 * SU2 International Developers Society <www.su2devsociety.org>
 * with selected contributions from the open-source community.
 *
 * The main research teams contributing to the current release are:
 *  - Prof. Juan J. Alonso's group at Stanford University.
 *  - Prof. Piero Colonna's group at Delft University of Technology.
 *  - Prof. Nicolas R. Gauger's group at Kaiserslautern University of Technology.
 *  - Prof. Alberto Guardone's group at Polytechnic University of Milan.
 *  - Prof. Rafael Palacios' group at Imperial College London.
 *  - Prof. Vincent Terrapon's group at the University of Liege.
 *  - Prof. Edwin van der Weide's group at the University of Twente.
 *  - Lab. of New Concepts in Aeronautics at Tech. Institute of Aeronautics.
 *
 * Copyright 2012-2019, Francisco D. Palacios, Thomas D. Economon,
 *                      Tim Albring, and the SU2 contributors.
 *
 * SU2 is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * SU2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with SU2. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "CScalarSolver.hpp"

/*!
 * \class CFlameletSolver
 * \brief Main class for defining the flamelet solver.
 * \ingroup Scalar_Model
 * \author D. Mayer, T. Economon, N.Beishuizen, G. Tamanampudi
 */
class CFlameletSolver: public CScalarSolver {
private:
  CFluidModel *FluidModel;     /*!< \brief Fluid model for the calar transport problem. */
  vector<string> scalar_names; /*!< \brief vector to store names of scalar variables.   */
  
public:
  /*!
   * \brief Constructor of the class.
   */
  CFlameletSolver(void);
  
  /*!
   * \overload
   * \param[in] geometry - Geometrical definition of the problem.
   * \param[in] config - Definition of the particular problem.
   * \param[in] iMesh - Index of the mesh in multigrid computations.
   * \param[in] FluidModel
   */
  CFlameletSolver(CGeometry *geometry, CConfig *config,
                          unsigned short iMesh);
  
  /*!
   * \brief Destructor of the class.
   */
  ~CFlameletSolver(void);
  
  /*!
   * \brief Restart residual and compute gradients.
   * \param[in] geometry - Geometrical definition of the problem.
   * \param[in] solver_container - Container vector with all the solutions.
   * \param[in] config - Definition of the particular problem.
   * \param[in] iMesh - Index of the mesh in multigrid computations.
   * \param[in] iRKStep - Current step of the Runge-Kutta iteration.
   * \param[in] RunTime_EqSystem - System of equations which is going to be solved.
   * \param[in] Output - boolean to determine whether to print output.
   */
  void Preprocessing(CGeometry *geometry, CSolver **solver_container,
                     CConfig *config, unsigned short iMesh,
                     unsigned short iRKStep, unsigned short RunTime_EqSystem,
                     bool Output);
  
  /*!
   * \brief Post-processing routine for the passive scalar model.
   * \param[in] geometry - Geometrical definition of the problem.
   * \param[in] solver_container - Container vector with all the solutions.
   * \param[in] config - Definition of the particular problem.
   */
  void Postprocessing(CGeometry *geometry, CSolver **solver_container,
                      CConfig *config, unsigned short iMesh);
  
  /*!
   * \brief Compute the primitive variables (diffusivities)
   * \param[in] solver_container - Container vector with all the solutions.
   * \param[in] config - Definition of the particular problem.
   * \param[in] Output - boolean to determine whether to print output.
   * \return - The number of non-physical points.
   */
  unsigned long SetPrimitive_Variables(CSolver **solver_container, CConfig *config, bool Output);
  
  /*!
   * \brief Set the initial condition for the scalar transport problem.
   * \param[in] geometry - Geometrical definition of the problem.
   * \param[in] solver_container - Container with all the solutions.
   * \param[in] config - Definition of the particular problem.
   * \param[in] ExtIter - External iteration.
   */
  void SetInitialCondition(CGeometry **geometry, CSolver ***solver_container,
                           CConfig *config, unsigned long ExtIter);
  
  /*!
   * \brief Compute the preconditioner for low-Mach flows.
   * \param[in] geometry - Geometrical definition of the problem.
   * \param[in] solver_container - Container vector with all the solutions.
   * \param[in] config - Definition of the particular problem.
   */
  void SetPreconditioner(CGeometry *geometry, CSolver **solver_container, CConfig *config);
  
  /*!
   * \brief Source term computation.
   * \param[in] geometry - Geometrical definition of the problem.
   * \param[in] solver_container - Container vector with all the solutions.
   * \param[in] numerics - Description of the numerical method.
   * \param[in] second_numerics - Description of the second numerical method.
   * \param[in] config - Definition of the particular problem.
   * \param[in] iMesh - Index of the mesh in multigrid computations.
   */
  void Source_Residual(CGeometry *geometry, CSolver **solver_container,
                       CNumerics *numerics, CNumerics *second_numerics,
                       CConfig *config, unsigned short iMesh);

  /*!
   * \brief Store the names of scalar variables that are being solved
   * \param[out] stores the names in vector scalar_names
   */
  inline void SetScalarNames(vector<string> val_names) {scalar_names = val_names;}
  
  /*!
   * \brief Get the name of scalar variable requested
   * \param[out] returns the name of scalar variable stored in the vector scalar_names
   */
  inline string GetScalarName(int val_ivar) {return scalar_names[val_ivar];}

  /*!
   * \brief Impose the Navier-Stokes wall boundary condition.
   * \param[in] geometry - Geometrical definition of the problem.
   * \param[in] solver_container - Container vector with all the solutions.
   * \param[in] conv_numerics - Description of the numerical method.
   * \param[in] visc_numerics - Description of the numerical method.
   * \param[in] config - Definition of the particular problem.
   * \param[in] val_marker - Surface marker where the boundary condition is applied.
   */
  void BC_Isothermal_Wall(CGeometry *geometry, CSolver **solver_container,
                          CNumerics *conv_numerics, CNumerics *visc_numerics,
                          CConfig *config, unsigned short val_marker);

  /*!
   * \brief Impose the inlet boundary condition.
   * \param[in] geometry - Geometrical definition of the problem.
   * \param[in] solver_container - Container vector with all the solutions.
   * \param[in] conv_numerics - Description of the numerical method.
   * \param[in] visc_numerics - Description of the numerical method.
   * \param[in] config - Definition of the particular problem.
   * \param[in] val_marker - Surface marker where the boundary condition is applied.
   */
  void BC_Inlet(CGeometry *geometry, CSolver **solver_container,
                CNumerics *conv_numerics, CNumerics *visc_numerics,
                CConfig *config, unsigned short val_marker);

  /*!
   * \brief Impose the outlet boundary condition.
   * \param[in] geometry - Geometrical definition of the problem.
   * \param[in] solver_container - Container vector with all the solutions.
   * \param[in] conv_numerics - Description of the numerical method.
   * \param[in] visc_numerics - Description of the numerical method.
   * \param[in] config - Definition of the particular problem.
   * \param[in] val_marker - Surface marker where the boundary condition is applied.
   */
  void BC_Outlet(CGeometry *geometry, CSolver **solver_container,
                 CNumerics *conv_numerics, CNumerics *visc_numerics,
                 CConfig *config, unsigned short val_marker);

};