//  ---------------------- Doxygen info ----------------------
//! \file 03_RMLPositionSampleApplication.cpp
//!
//! \brief
//! Test application number 3 for the Reflexxes Motion Libraries
//! (position-based interface, time- and phase-synchronization)
//!
//! \date March 2014
//!
//! \version 1.2.6
//!
//! \author Torsten Kroeger, <info@reflexxes.com> \n
//!
//! \copyright Copyright (C) 2014 Google, Inc.
//! \n
//! \n
//! <b>GNU Lesser General Public License</b>
//! \n
//! \n
//! This file is part of the Type II Reflexxes Motion Library.
//! \n\n
//! The Type II Reflexxes Motion Library is free software: you can redistribute
//! it and/or modify it under the terms of the GNU Lesser General Public License
//! as published by the Free Software Foundation, either version 3 of the
//! License, or (at your option) any later version.
//! \n\n
//! The Type II Reflexxes Motion Library is distributed in the hope that it
//! will be useful, but WITHOUT ANY WARRANTY; without even the implied
//! warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
//! the GNU Lesser General Public License for more details.
//! \n\n
//! You should have received a copy of the GNU Lesser General Public License
//! along with the Type II Reflexxes Motion Library. If not, see
//! <http://www.gnu.org/licenses/>.
//  ----------------------------------------------------------
//   For a convenient reading of this file's source code,
//   please use a tab width of four characters.
//  ----------------------------------------------------------


#include <stdio.h>
#include <stdlib.h>

#include <ReflexxesAPI.h>
#include <RMLPositionFlags.h>
#include <RMLPositionInputParameters.h>
#include <RMLPositionOutputParameters.h>


//*************************************************************************
// defines

#define CYCLE_TIME_IN_SECONDS                   0.001
#define NUMBER_OF_DOFS                          2


//*************************************************************************
// Main function to run the process that contains the test application
//
// This function is based on the sample program of
// 01_RMLPositionSampleApplication.cpp and contains a simple example to use
// and distinguish between time- and phase-synchronized motion trajectories
// that are generated by the Type II Reflexxes Motion Library. This code
// snippet directly corresponds to the example trajectories shown in the
// documentation.
//*************************************************************************
int main()
{
    // ********************************************************************
    // Variable declarations and definitions

    bool                        IntermediateTargetStateSet  =   false
                            ,   IntermediateStateReached    =   false   ;

    int                         ResultValue                 =   0       ;

    double                      Time                        =   0.0     ;

    ReflexxesAPI                *RML                        =   NULL    ;

    RMLPositionInputParameters  *IP                         =   NULL    ;

    RMLPositionOutputParameters *OP                         =   NULL    ;

    RMLPositionFlags            Flags                                   ;

    // ********************************************************************
    // Creating all relevant objects of the Type II Reflexxes Motion Library

    RML =   new ReflexxesAPI(                   NUMBER_OF_DOFS
                                            ,   CYCLE_TIME_IN_SECONDS   );

    IP  =   new RMLPositionInputParameters(     NUMBER_OF_DOFS          );

    OP  =   new RMLPositionOutputParameters(    NUMBER_OF_DOFS          );

    // ********************************************************************
    // Set-up a timer with a period of one millisecond
    // (not implemented in this example in order to keep it simple)
    // ********************************************************************

    printf("-------------------------------------------------------\n"  );
    printf("Reflexxes Motion Libraries                             \n"  );
    printf("Example: 03_RMLPositionSampleApplication               \n\n");
    printf("This example demonstrates how to set-up the            \n"  );
    printf("generation of time- and phase-synchronized motion      \n"  );
    printf("trajectories using the position-based Online           \n"  );
    printf("Trajectory Generation algorithm of the Type II         \n"  );
    printf("Reflexxes Motion Library.                              \n\n");
    printf("Copyright (C) 2014 Google, Inc.                      \n"  );
    printf("-------------------------------------------------------\n"  );

    // ********************************************************************
    // Set-up the input parameters

    // In this test program, arbitrary values are chosen. If executed on a
    // real robot or mechanical system, the position is read and stored in
    // an RMLPositionInputParameters::CurrentPositionVector vector object.
    // For the very first motion after starting the controller, velocities
    // and acceleration are commonly set to zero. The desired target state
    // of motion and the motion constraints depend on the robot and the
    // current task/application.
    // The internal data structures make use of native C data types
    // (e.g., IP->CurrentPositionVector->VecData is a pointer to
    // an array of NUMBER_OF_DOFS double values), such that the Reflexxes
    // Library can be used in a universal way.

    IP->CurrentPositionVector->VecData      [0] =    100.0      ;
    IP->CurrentPositionVector->VecData      [1] =    100.0      ;

    IP->CurrentVelocityVector->VecData      [0] =      0.0      ;
    IP->CurrentVelocityVector->VecData      [1] =      0.0      ;

    IP->CurrentAccelerationVector->VecData  [0] =      0.0      ;
    IP->CurrentAccelerationVector->VecData  [1] =      0.0      ;

    IP->MaxVelocityVector->VecData          [0] =    300.0      ;
    IP->MaxVelocityVector->VecData          [1] =    300.0      ;

    IP->MaxAccelerationVector->VecData      [0] =    400.0      ;
    IP->MaxAccelerationVector->VecData      [1] =    400.0      ;

    IP->MaxJerkVector->VecData              [0] =    500.0      ;
    IP->MaxJerkVector->VecData              [1] =    500.0      ;

    IP->TargetPositionVector->VecData       [0] =    700.0      ;
    IP->TargetPositionVector->VecData       [1] =    300.0      ;

    IP->TargetVelocityVector->VecData       [0] =     0.0       ;
    IP->TargetVelocityVector->VecData       [1] =     0.0       ;

    IP->SelectionVector->VecData            [0] =   true        ;
    IP->SelectionVector->VecData            [1] =   true        ;

    // ********************************************************************
    // Setting the flag for time- and phase-synchronization:
    //
    //  - RMLPositionFlags::ONLY_TIME_SYNCHRONIZATION for
    //    time-synchronization
    //  - RMLPositionFlags::PHASE_SYNCHRONIZATION_IF_POSSIBLE for
    //    phase-synchronization
    //
    // Please feel free to change this flag to see the difference in the
    // behavior of the algorithm.

    Flags.SynchronizationBehavior   =   RMLPositionFlags::ONLY_TIME_SYNCHRONIZATION;

    // ********************************************************************
    // Starting the control loop

    for(;;)
    {

        // ****************************************************************
        // Wait for the next timer tick
        // (not implemented in this example in order to keep it simple)
        // ****************************************************************

        // Calling the Reflexxes OTG algorithm
        ResultValue =   RML->RMLPosition(       *IP
                                            ,   OP
                                            ,   Flags       );

        if (ResultValue < 0)
        {
            printf("An error occurred (%d).\n", ResultValue );
            break;
        }

        // ****************************************************************
        // Here, the new state of motion, that is
        //
        // - OP->NewPositionVector
        // - OP->NewVelocityVector
        // - OP->NewAccelerationVector
        //
        // can be used as input values for lower level controllers. In the
        // most simple case, a position controller in actuator space is
        // used, but the computed state can be applied to many other
        // controllers (e.g., Cartesian impedance controllers,
        // operational space controllers).
        // ****************************************************************

        // ****************************************************************
        // Feed the output values of the current control cycle back to
        // input values of the next control cycle

        *IP->CurrentPositionVector      =   *OP->NewPositionVector      ;
        *IP->CurrentVelocityVector      =   *OP->NewVelocityVector      ;
        *IP->CurrentAccelerationVector  =   *OP->NewAccelerationVector  ;

        Time    +=  CYCLE_TIME_IN_SECONDS;

        // ****************************************************************
        // In this introductory example, we simple trigger a sensor event
        // after one second. On a real-world system, trigger signal are
        // commonly generated based on (unforeseen) sensor signals. This
        // event changes the input parameters and specifies a
        // intermediate state of motion, that is, a new desired target
        // state of motion for the Reflexxes algorithm.


        if (    (   Time >= 1.0                 )
            &&  (   !IntermediateTargetStateSet )   )
        {
            IP->TargetPositionVector->VecData       [0] =    550.0      ;
            IP->TargetPositionVector->VecData       [1] =    250.0      ;

            IP->TargetVelocityVector->VecData       [0] =   -150.0      ;
            IP->TargetVelocityVector->VecData       [1] =    -50.0      ;
        }

        // ****************************************************************
        // After reaching the intermediate state of motion define above
        // we switch the values of the desired target state of motion
        // back to the original one. In the documentation and the
        // description of time- and phase-synchronized motion trajectories,
        // this switching happens at 3873 milliseconds.

        if (    (   ResultValue == ReflexxesAPI::RML_FINAL_STATE_REACHED    )
            &&  (   !IntermediateStateReached                               )   )
        {
            IntermediateStateReached    =   true;

            IP->TargetPositionVector->VecData       [0] =    700.0      ;
            IP->TargetPositionVector->VecData       [1] =    300.0      ;

            IP->TargetVelocityVector->VecData       [0] =     0.0       ;
            IP->TargetVelocityVector->VecData       [1] =     0.0       ;

            continue;
        }

        // ****************************************************************
        // After the final state of motion is reached, we leave the loop
        // and terminate the program.

        if (ResultValue == ReflexxesAPI::RML_FINAL_STATE_REACHED)
        {
            break;
        }
    }

    // ********************************************************************
    // Deleting the objects of the Reflexxes Motion Library end terminating
    // the process

    delete  RML         ;
    delete  IP          ;
    delete  OP          ;

    exit(EXIT_SUCCESS)  ;
}
