/*
 *
 *                 #####    #####   ######  ######  ###   ###
 *               ##   ##  ##   ##  ##      ##      ## ### ##
 *              ##   ##  ##   ##  ####    ####    ##  #  ##
 *             ##   ##  ##   ##  ##      ##      ##     ##
 *            ##   ##  ##   ##  ##      ##      ##     ##
 *            #####    #####   ##      ######  ##     ##
 *
 *
 *             OOFEM : Object Oriented Finite Element Code
 *
 *               Copyright (C) 1993 - 2013   Borek Patzak
 *
 *
 *
 *       Czech Technical University, Faculty of Civil Engineering,
 *   Department of Structural Mechanics, 166 29 Prague, Czech Republic
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef ConcreteDPM2_h
#define ConcreteDPM2_h

#include "sm/Materials/structuralmaterial.h"
#include "floatarray.h"
#include "floatmatrix.h"
#include "cltypes.h"
#include "sm/Materials/structuralms.h"
#include "sm/Materials/isolinearelasticmaterial.h"
#include "gausspoint.h"
#include "mathfem.h"

#define CDPM2_TOL 1.e-6
#define keep_track_of_dissipated_energy
///@name Input fields for ConcreteDPM2
//@{
#define _IFT_ConcreteDPM2_Name "con2dpm"
#define _IFT_ConcreteDPM2_fc "fc"
#define _IFT_ConcreteDPM2_ft "ft"
#define _IFT_ConcreteDPM2_ecc "ecc"
#define _IFT_ConcreteDPM2_kinit "kinit"
#define _IFT_ConcreteDPM2_ahard "ahard"
#define _IFT_ConcreteDPM2_bhard "bhard"
#define _IFT_ConcreteDPM2_chard "chard"
#define _IFT_ConcreteDPM2_dhard "dhard"
#define _IFT_ConcreteDPM2_dilation "dilation"
#define _IFT_ConcreteDPM2_asoft "asoft"
#define _IFT_ConcreteDPM2_hp "hp"
#define _IFT_ConcreteDPM2_yieldtol "yieldtol"
#define _IFT_ConcreteDPM2_newtoniter "newtoniter"
#define _IFT_ConcreteDPM2_wf "wf"
#define _IFT_ConcreteDPM2_efc "efc"
#define _IFT_ConcreteDPM2_softeningType "stype"
#define _IFT_ConcreteDPM2_ftOne "ft1"
#define _IFT_ConcreteDPM2_wfOne "wf1"
#define _IFT_ConcreteDPM2_rateFlag "rateflag"
#define _IFT_ConcreteDPM2_deltatime "deltat"
#define _IFT_ConcreteDPM2_helem "helem"
#define _IFT_ConcreteDPM2_isoflag "isoflag"
//@}

namespace oofem {
/**
 * This class implements the material status associated to ConcreteDPM2.
 * @author Peter Grassl, Dimitrios Xenos
 */
class ConcreteDPM2Status : public StructuralMaterialStatus
{
public:
    /// Values of history variable state_flag.
    enum state_flag_values {
        ConcreteDPM2_Elastic,
        ConcreteDPM2_Unloading,
        ConcreteDPM2_Plastic,
        ConcreteDPM2_Damage,
        ConcreteDPM2_PlasticDamage,
        ConcreteDPM2_VertexCompression,
        ConcreteDPM2_VertexTension,
        ConcreteDPM2_VertexCompressionDamage,
        ConcreteDPM2_VertexTensionDamage
    };
protected:
    /// @name History variables of the plasticity model
    //@{
    FloatArray plasticStrain;
    FloatArray tempPlasticStrain;

    FloatArray reducedStrain;
    FloatArray tempReducedStrain;

    //@}

    /// @name Hardening variable
    //@{
    double kappaP = 0.;
    double tempKappaP = 0.;
    //@}

    double kappaPPeak = 0.;

    double le = 0.;

    double alpha = 0.;
    double tempAlpha = 0.;

    double equivStrain = 0.;
    double tempEquivStrain = 0.;

    double equivStrainTension = 0.;
    double tempEquivStrainTension = 0.;

    double equivStrainCompression = 0.;
    double tempEquivStrainCompression = 0.;

    double kappaDTension = 0.;
    double tempKappaDTension = 0.;

    double kappaDCompression = 0.;
    double tempKappaDCompression = 0.;

    double kappaDTensionOne = 0.;
    double tempKappaDTensionOne = 0.;

    double kappaDCompressionOne = 0.;
    double tempKappaDCompressionOne = 0.;

    double kappaDTensionTwo = 0.;
    double tempKappaDTensionTwo = 0.;

    double kappaDCompressionTwo = 0.;
    double tempKappaDCompressionTwo = 0.;

    double damageTension = 0.;
    double tempDamageTension = 0.;

    double damageCompression = 0.;
    double tempDamageCompression = 0.;

    double deltaEquivStrain = 0.;

    double rateFactor = 1.;
    double tempRateFactor = 0.;

    /// Strains that are used for calculation of strain rates
    double rateStrain = 0.;
    double tempRateStrain = 0.;

    /// Indicates the state (i.e. elastic, unloading, plastic, damage, vertex) of the Gauss point
    int state_flag = ConcreteDPM2Status :: ConcreteDPM2_Elastic;
    int temp_state_flag = ConcreteDPM2Status :: ConcreteDPM2_Elastic;
#ifdef keep_track_of_dissipated_energy
    /// Density of total work done by stresses on strain increments.
    double stressWork = 0.;
    /// Non-equilibrated density of total work done by stresses on strain increments.
    double tempStressWork = 0.;
    /// Density of dissipated work.
    double dissWork = 0.;
    /// Non-equilibrated density of dissipated work.
    double tempDissWork = 0.;
#endif

public:
    /// Constructor
    ConcreteDPM2Status(GaussPoint *gp);

    void initTempStatus() override;
    void updateYourself(TimeStep *tStep) override;
    void printOutputAt(FILE *file, TimeStep *tStep) const override;
    void saveContext(DataStream &stream, ContextMode mode) override;
    void restoreContext(DataStream &stream, ContextMode mode) override;
    const char *giveClassName() const override { return "ConcreteDPM2Status"; }

    // Inline functions for access to state variables

    /**
     * Get the reduced strain vector from the material status.
     * @return Reduced strain vector.
     */
    const FloatArray &giveReducedStrain() const { return reducedStrain; }

    /**
     * Get the reduced strain vector from the material status.
     * @return Reduced strain vector.
     */
    const FloatArray &giveTempReducedStrain() const { return tempReducedStrain; }


    /**
     * Get the plastic strain vector from the material status.
     * @return Plastic strain vector.
     */
    const FloatArray &givePlasticStrain() const { return plasticStrain; }

    /**
     * Get the deviatoric plastic strain norm from the material status.
     * @return DeviatoricPlasticStrainNorm.
     */
    double giveDeviatoricPlasticStrainNorm()
    {
        auto dev = StructuralMaterial :: computeDeviator(plasticStrain);
        return sqrt( .5 * ( 2. * dev [ 0 ] * dev [ 0 ] + 2. * dev [ 1 ] * dev [ 1 ] + 2. * dev [ 2 ] * dev [ 2 ] +
                    dev [ 3 ] * dev [ 3 ] + dev [ 4 ] * dev [ 4 ] + dev [ 5 ] * dev [ 5 ] ) );
    }

    /**
     * Get the volumetric plastic strain from the material status.
     * @return volumetricPlasticStrainNorm.
     */
    double giveVolumetricPlasticStrain() const
    {
        return 1. / 3. * ( plasticStrain(0) + plasticStrain(1) + plasticStrain(2) );
    }

    /**
     * Get the hardening variable of the plasticity model.
     * @return The hardening variable of the plasticity model.
     */
    double giveKappaP() const
    { return kappaP; }

    /**
     * Get the hardening variable of the damage model from the
     * material status.
     * @return Hardening variable kappaD.
     */
    double giveKappaDTensionOne() const
    { return kappaDTensionOne; }

    /**
     * Get the compression hardening variable one of the damage model from the
     * material status.
     * @return Hardening variable kappaDCompressionOne.
     */
    double giveKappaDCompressionOne() const
    { return kappaDCompressionOne; }


    /**
     * Get the tension hardening variable two of the damage model from the
     * material status.
     * @return Hardening variable kappaDTensionTwo.
     */
    double giveKappaDTensionTwo() const
    { return kappaDTensionTwo; }


    /**
     * Get the compression hardening variable two of the damage model from the
     * material status.
     * @return Hardening variable kappaDCompressionTwo.
     */
    double giveKappaDCompressionTwo() const
    { return kappaDCompressionTwo; }


    /**
     * Get the equivalent strain from the
     * material status.
     * @return Equivalent strain equivStrain.
     */
    double giveEquivStrain() const
    { return equivStrain; }

    /**
     * Get the tension equivalent strain from the
     * material status.
     * @return Equivalent strain equivStrainTension.
     */
    double giveEquivStrainTension() const
    { return equivStrainTension; }


    /**
     * Get the compression equivalent strain from the
     * material status.
     * @return Equivalent strain equivStrainCompression.
     */
    double giveEquivStrainCompression() const
    { return equivStrainCompression; }

    /**
     * Get the tension damage variable of the damage model from the
     * material status.
     * @return Tension damage variable damageTension.
     */
    double giveDamageTension() const
    { return damageTension; }

    /**
     * Get the compressive damage variable of the damage model from the
     * material status.
     * @return Compressive damage variable damageCompression.
     */
    double giveDamageCompression() const
    { return damageCompression; }

    /**
     * Get the rate factor of the damage model from the
     * material status.
     * @return rate factor rateFactor.
     */
    double giveRateFactor() const
    { return rateFactor; }

    /**
     * Get the temp variable of the damage model from the
     * material status.
     * @return Damage variable damage.
     */
    double giveTempRateFactor() const
    { return tempRateFactor; }


    double giveRateStrain() const
    { return rateStrain; }

    void letTempRateStrainBe(double v)
    { tempRateStrain = v; }


    void letTempAlphaBe(double v)
    { tempAlpha = v; }

    /**
     * Get the state flag from the material status.
     * @return State flag (i.e. elastic, unloading, yielding, vertex case yielding)
     */
    int giveStateFlag() const
    { return state_flag; }

    // giveTemp:

    // Functions used to access the temp variables.
    /**
     * Get the temp value of the full plastic strain vector from the material status.
     * @return Temp value of plastic strain vector.
     */
    const FloatArray &giveTempPlasticStrain() const { return tempPlasticStrain; }

    /**
     *  Get the temp value of the volumetric plastic strain in plane stress
     */
    double giveTempVolumetricPlasticStrain() const
    { return 1. / 3. * ( tempPlasticStrain(0) + tempPlasticStrain(1) + tempPlasticStrain(2) ); }

    /**
     * Get the temp value of the hardening variable of the plasticity model
     * from the material status.
     * @return Temp value of hardening variable kappaP.
     */
    double giveTempKappaP() const
    { return tempKappaP; }

    /**
     * Get the temp value of the hardening variable of the damage model
     * from the material status.
     * @return Temp value of the damage variable damage.
     */
    double giveKappaDTension() const
    { return kappaDTension; }

    double giveAlpha() const
    { return alpha; }

    /**
     * Get the temp value of the hardening variable of the damage model
     * from the material status.
     * @return Temp value of the damage variable damage.
     */
    double giveKappaDCompression() const
    { return kappaDCompression; }

    /**
     * Get the temp value of the hardening variable of the damage model
     * from the material status.
     * @return Temp value of the damage variable damage.
     */
    double giveTempDamageTension() const
    { return tempDamageTension; }

    /**
     * Get the temp value of the hardening variable of the damage model
     * from the material status.
     * @return Temp value of the damage variable damage.
     */
    double giveTempDamageCompression() const
    { return tempDamageCompression; }

    /**
     * Get the temp value of the hardening variable of the damage model
     * from the material status.
     * @return Temp value of the damage variable damage.
     */
    double giveDeltaEquivStrain() const
    { return deltaEquivStrain; }

    /**
     * Get the temp value of the state flag from the material status.
     * @return Temp value of the state flag (i.e. elastic, unloading,
     * yielding, vertex case yielding).
     */
    int giveTempStateFlag() const
    { return temp_state_flag; }

    // letTemp...be :
    // Functions used by the material to assign a new value to a temp variable.
    /**
     * Assign the temp value of deviatoric plastic strain.
     * @param v New temp value of deviatoric plastic strain
     */
    void letTempPlasticStrainBe(const FloatArray &v)
    { tempPlasticStrain = v; }


    void letTempReducedStrainBe(const FloatArray &v)
    { tempReducedStrain = v; }


    /**
     * Assign the temp value of the hardening variable of the plasticity model.
     * @param v New temp value of the hardening variable
     */
    void letTempKappaPBe(double v)
    { tempKappaP = v; }

    /**
     * Assign the temp value of the rate factor of the damage model.
     * @param v New temp value of the damage variable
     */
    void letTempKappaDTensionBe(double v)
    { tempKappaDTension = v; }

    /**
     * Assign the temp value of the rate factor of the damage model.
     * @param v New temp value of the damage variable
     */
    void letTempKappaDCompressionBe(double v)
    { tempKappaDCompression = v; }

    /**
     * Assign the temp value of the hardening variable of the damage model.
     * @param v New temp value of the hardening variable
     */
    void letTempKappaDTensionOneBe(double v)
    { tempKappaDTensionOne = v; }

    /**
     * Assign the temp value of the hardening variable of the damage model.
     * @param v New temp value of the hardening variable
     */
    void letTempKappaDCompressionOneBe(double v)
    { tempKappaDCompressionOne = v; }

    /**
     * Assign the temp value of the second tension hardening variable of the damage model.
     * @param v New temp value of the second tension hardening variable
     */
    void letTempKappaDTensionTwoBe(double v)
    { tempKappaDTensionTwo = v; }

    /**
     * Assign the temp value of the second compression hardening variable of the damage model.
     * @param v New temp value of the second compression hardening variable
     */
    void letTempKappaDCompressionTwoBe(double v)
    { tempKappaDCompressionTwo = v; }

    /**
     * Assign the temp value of the tensile damage variable of the damage model.
     * @param v New temp value of the tensile damage variable
     */
    void letTempDamageTensionBe(double v)
    { tempDamageTension = v; }

    /**
     * Assign the temp value of the compressive damage variable of the damage model.
     * @param v New temp value of the compressive damage variable
     */
    void letTempDamageCompressionBe(double v)
    { tempDamageCompression = v; }

    /**
     * Assign the temp value of the rate factor of the damage model.
     * @param v New temp value of the damage variable
     */
    void letTempRateFactorBe(double v)
    { tempRateFactor = v; }

    /**
     * Assign the temp value of the rate factor of the damage model.
     * @param v New temp value of the damage variable
     */
    void letTempEquivStrainBe(double v)
    { tempEquivStrain = v; }

    /**
     * Assign the temp value of the rate factor of the damage model.
     * @param v New temp value of the damage variable
     */
    void letTempEquivStrainTensionBe(double v)
    { tempEquivStrainTension = v; }

    /**
     * Assign the temp value of the rate factor of the damage model.
     * @param v New temp value of the damage variable
     */
    void letTempEquivStrainCompressionBe(double v)
    { tempEquivStrainCompression = v; }

    /**
     *  Gives the characteristic length.
     */
    double giveLe() { return le; }

    /**
     *  Sets the characteristic length.
     */
    void setLe(double ls)
    { le = ls; }
    /**
     * Assign the temp value of the state flag.
     * @param v New temp value of the state flag (i.e. elastic, unloading, yielding,
     * vertex case yielding).
     */
    void letTempStateFlagBe(const int v)
    { temp_state_flag = v; }

    void letKappaPPeakBe(double kappa)
    { kappaPPeak = kappa; }
#ifdef keep_track_of_dissipated_energy
    /// Returns the density of total work of stress on strain increments.
    double giveStressWork() { return stressWork; }
    /// Returns the temp density of total work of stress on strain increments.
    double giveTempStressWork() { return tempStressWork; }
    /// Sets the density of total work of stress on strain increments to given value.
    void setTempStressWork(double w) { tempStressWork = w; }
    /// Returns the density of dissipated work.
    double giveDissWork() { return dissWork; }
    /// Returns the density of temp dissipated work.
    double giveTempDissWork() { return tempDissWork; }
    /// Sets the density of dissipated work to given value.
    void setTempDissWork(double w) { tempDissWork = w; }
    /**
     * Computes the increment of total stress work and of dissipated work
     * (gf is the dissipation density per unit volume at complete failure,
     * it is needed only to determine which extremely small dissipation
     * can be set to zero to get clean results, but parameter gf can be
     * set to zero if not available).
     */
    void computeWork(GaussPoint *gp, double ft);
#endif
};


//   ********************************
//   *** CLASS DYNAMIC CONCRETE   ***
//   ********************************

/**
 * This class contains the combination of a local plasticity model for concrete with a local isotropic damage model.
 * This is an extension of concretedpm2. The yield surface of the plasticity model is based on the extension of the Menetrey and Willam yield criterion.
 * The flow rule is nonassociated. The evolution laws of the hardening variables depend on the stress state.
 * The plasticity model describes only hardening and perfect plasticity.
 * It is based on the effective stress. The damage parameter of the isotropic damage model is based on the total volumetric strain.
 * An exponential softening law is implemented.
 *
 * @author Peter Grassl, Dimitrios Xenos
 */
class ConcreteDPM2 : public StructuralMaterial
{
public:

protected:
    enum ConcreteDPM2_ReturnType { RT_Regular, RT_Tension, RT_Compression, RT_Auxiliary };
    ConcreteDPM2_ReturnType returnType;

    enum ConcreteDPM2_ReturnResult { RR_NotConverged, RR_Converged };
    ConcreteDPM2_ReturnResult returnResult;

    /// Parameters of the yield surface of the plasticity model. fc is the uniaxial compressive strength, ft the uniaxial tensile strength and ecc controls the out of roundness of the deviatoric section.
    double fc, ft, ecc;

    int isotropicFlag;

    double e0;

    /// Parameter of the ductilityMeasure of the plasticity model.
    double AHard;
    /// Parameter of the ductilityMeasure of the plasticity model.
    double BHard;
    /// Parameter of the ductilityMeasure of the plasticity model.
    double CHard;
    /// Parameter of the ductilityMeasure of the plasticity model.
    double DHard;

    /// Hardening modulus.
    double hardeningModulus;

    /// Parameter of the ductilityMeasure of the damage model.
    double ASoft;

    /// Parameter of the hardening law of the plasticity model.
    double yieldHardPrimePeak;

    /// Parameter of the hardening law of the plasticity model.
    double yieldHardInitial;

    /// Control parameter for te volumetric plastic flow of the plastic potential.
    double dilationConst;

    /// Friction parameter of the yield surface.
    double m;

    /// Dilation parameter of the plastic potential.
    double mQ;

    /// Element size (to be used in fracture energy approach (crack band).
    double helem;

    /// Pointer for linear elastic material.
    IsotropicLinearElasticMaterial linearElasticMaterial;

    /// Elastic Young's modulus.
    double eM;
    /// Elastic shear modulus.
    double gM;
    /// Elastic bulk modulus.
    double kM;
    /// Elastic poisson's ration.
    double nu;

    /// Control parameter for the exponential softening law.
    double efCompression;

    /// Control parameter for the linear/bilinear softening law in tension.
    double wf;

    /// Control parameter for the bilinear softening law in tension.
    double wfOne;

    /// Control parameter for the bilinear softening law.
    double ftOne;

    /// yield tolerance for the plasticity model.
    double yieldTol;

    /// yield tolerance for the damage model.
    double yieldTolDamage;

    /// Maximum number of iterations for stress return.
    int newtonIter;

    /// Type of softening function used.
    int softeningType;

    /// Input parameter which simulates a loading rate. Only for debugging purposes.
    double deltaTime;

    /** Flag which signals if strainRate effects should be considered.
     * 0 = no strain rate (default)
     * 1 = mod. CEB strain rate effect for strength and squared for fracture energy
     * 2 = mod. CEB strain rate effect for strength and linear for fracture energy
     * 3 = mod. CEB strain rate effect for strength with constant fracture energy
     */
    int strainRateFlag;

public:
    /// Constructor
    ConcreteDPM2(int n, Domain *d);
    /// Destructor
    virtual ~ConcreteDPM2();
    void initializeFrom(InputRecord &ir) override;

    const char *giveClassName() const override { return "ConcreteDPM2"; }
    const char *giveInputRecordName() const override { return _IFT_ConcreteDPM2_Name; }

    void giveRealStressVector_1d(FloatArray &answer, GaussPoint *gp, const FloatArray &totalStrain, TimeStep *tStep) override;

    void giveRealStressVector_3d(FloatArray &answer, GaussPoint *gp, const FloatArray &strainVector, TimeStep *tStep) override;

    bool hasMaterialModeCapability(MaterialMode mode) const override;

    /**
     * Perform stress return of the plasticity model and compute history variables.
     * @param gp Gauss point.
     * @param D stiffness matrix
     * @param strain Strain vector of this Gauss point.
     */
    void performPlasticityReturn(GaussPoint *gp,
                                 const FloatMatrix &D,
                                 const FloatArray &strain,
                                 FloatArray &effectivestress);


    /**
     * Check if the trial stress state falls within the vertex region of the plasticity model at the apex of triaxial extension or triaxial compression.
     * @returns true for vertex case and false if regular stress return can be used.
     * @param answer Volumetric apex stress.
     * @param sig Volumetric stress.
     * @param tempKappa Hardening variable.
     * @param mode1d
     */
    bool checkForVertexCase(double &answer,
                            double sig,
                            double tempKappa,
                            bool mode1d);

    /**
     * Perform regular stress return for the plasticity model, i.e. if the trial stress state does not lie in the vertex region.
     * @param stress Stress vector which is computed.
     * @param kappaP Initial guess for kappa P (i.e. previous kappa)
     * @param gp Gauss point.
     * @param theta Load angle of trial stress (remains constant throughout return).
     */
    double performRegularReturn(FloatArray &stress,
                                double kappaP,
                                GaussPoint *gp,
                                double theta);

    /**
     * Compute jacobian for 1D case
     * @param totalsigma stress value
     * @param theta Lode angle
     * @param tempKappa plastic strain
     * @param deltaLambda plastic multiplier
     * @param gp Gauss point
     */

    void compute1dJacobian(FloatMatrix &answer,
                           double totalsigma,
                           double theta,
                           double tempKappa,
                           double deltaLambda,
                           GaussPoint *gp);
    /**
     * Compute jacobian for 2D(plane strain) and 3d cases
     * @param sig volumetric strain
     * @param rho deviatoric
     * @param tempKappa plastic strain
     * @param deltaLambda plastic multiplier
     * @param gp Gauss point
     */
    void computeJacobian(FloatMatrix &answer,
                         double sig,
                         double rho,
                         double theta,
                         double tempKappa,
                         double deltaLambda,
                         GaussPoint *gp);

    /**
     * Perform stress return for vertex case of the plasticity model, i.e. if the trial stress state lies within the vertex region.
     * @param stress Stress vector of this Gauss point.
     * @param apexStress Volumetric stress at the apex of the yield surface.
     * @param theta Lode angle.
     * @param tempKappaP temporary cummulative plastic strain
     * @param gp Gauss point.
     * @returns updated temporary cummulative plastic strain
     */
    double performVertexReturn(FloatArray &stress,
                               double apexStress,
                               double tempKappaP,
                               GaussPoint *gp);

    /**
     * Compute the yield value based on stress and hardening variable.
     * @param sig Volumetric stress.
     * @param rho Length of the deviatoric stress.
     * @param theta Lode angle of the stress state.
     * @param tempKappa Hardening variable.
     * @returns Yield value.
     */
    double computeYieldValue(double sig,
                             double rho,
                             double theta,
                             double tempKappa) const;

    /**
     * Compute the value of the hardening function based on the hardening variable.
     * @param tempKappa Hardening variable.
     * @returns Value of the hardening function.
     */
    double computeHardeningOne(double tempKappa) const;

    /** Compute the derivative of the hardening function based on the
     *  hardening parameter.
     *  @param tempKappa Hardening variable.
     *  @return Derivative of the hardening function.
     */
    double computeHardeningOnePrime(double tempKappa) const;

    /**
     * Compute the value of the hardening function based on the hardening variable.
     * @param tempKappa Hardening variable.
     * @return Value of the hardening function.
     */
    double computeHardeningTwo(double tempKappa) const;

    /**
     * Compute the derivative of the hardening function based on the
     * hardening parameter.
     * @param tempKappa Hardening variable.
     * @return Derivative of the hardening function.
     */
    double computeHardeningTwoPrime(double tempKappa) const;

    /** Compute the derivative of the yield surface with respect to the hardening
     *  variable based on the stress state and the hardening variable
     *  @param sig Volumetric stress.
     *  @param rho Deviatoric length.
     *  @param theta Lode angle.
     *  @param tempKappa Hardening variable.
     *  @return Derivative of the yield surface.
     */
    double computeDFDKappa(double sig,
                           double rho,
                           double theta,
                           double tempKappa,
                           bool mode1d);

    /**
     * 3D: Compute the derivative of kappa with respect of delta lambda based on the stress state and the hardening variable.
     * @param sig Volumetric stress.
     * @param rho Length of the deviatoric stress.
     * @param theta Lode angle
     * @param tempKappa Hardening variable.
     * @return Derivative of kappa with respect to delta lambda.
     */
    double computeDKappaDDeltaLambda(double sig, double rho, double theta, double tempKappa);

    /**
     * 1D: Compute the derivative of kappa with respect of delta lambda based on the stress state and the hardening variable.
     * @param sig Volumetric stress.
     * @param theta Lode angle
     * @param tempKappa Hardening variable.
     * @return Derivative of kappa with respect to delta lambda.
     */
    double computeDKappaDDeltaLambda1d(double sig, double theta, double tempKappa);

    /**
     * Compute the ductility measure based on the stress state.
     * @param sig Volumetric stress.
     * @param rho Length of the deviatoric strength.
     * @param theta Lode angle of stress state.
     * @return Ductility measure.
     */
    virtual double computeDuctilityMeasure(double sig,
                                           double rho,
                                           double theta);

    /**
     * Compute derivative the ductility measure with respect to  the stress state.
     * @param answer array of the derivative of the ductility measure with respect to volumetric and deviatoric stress
     * @param sig Volumetric stress.
     * @param rho Length of the deviatoric strength.
     * @param theta Lode angle.
     * @param tempKappa plastic strain
     */
    void computeDDuctilityMeasureDInv(FloatArray &answer,
                                      double sig,
                                      double rho,
                                      double theta,
                                      double tempKappa);
    /**
     * Compute derivative the ductility measure with respect to  the stress state.
     * @return The derivative of the ductility measure with respect to stress
     * @param sig stress.
     * @param theta Lode angle.
     * @param tempKappa plastic strain
     */
    double computeDDuctilityMeasureDInv1d(double sigma, double theta, double tempKappa); //Dimitris change 1d implementation

    /**
     * Compute derivative the palstic potential function with respect to  the stress state.
     * @param answer array of the derivative of the plastic potential with respect to volumetric and deviatoric stress
     * @param sig volumetric stress.
     * @param rho deviatoric stress.
     * @param tempKappa plastic strain
     */
    void computeDGDInv(FloatArray &answer,
                       double sig,
                       double rho,
                       double tempKappa);
    /**
     * Compute derivative the palstic potential function with respect to  the stress state.
     * @param return The derivative of the plastic potential with respect to stress
     * @param sig stress.
     * @param tempKappa hardening variable
     */
    double computeDGDInv1d(double sig, double tempKappa);

    /**
     * This function computes the ratio of the volumetric and deviatoric component
     * of the flow direction. It is used within the vertex return to check,
     * if the vertex return is admissible.
     */
    double computeRatioPotential(double sig,
                                 double rho,
                                 double tempKappa);

    /**
     * This function computes the rate factor which is used to take into account the strain rate dependence of the material.
     */
    double computeRateFactor(double alpha,
                             double timeFactor,
                             GaussPoint *gp,
                             TimeStep *deltaTime);

    /**
     * 3D: Second derivative of the plastic potential with respect to the
     * invariants sig and rho are computed.
     */
    void computeDDGDDInv(FloatMatrix &answer,
                         double sig,
                         double rho,
                         double tempKappa);
    /**
     * 1D: The second derivative of the plastic potential with respect to the
     * invariants sig and rho are computed.
     */
    double computeDDGDDInv1d(double sigma, double tempKappa);

    /**
     * 3D: The mixed derivative of the plastic potential with respect
     * to the invariants and the hardening parameter are determined.
     */
    void computeDDGDInvDKappa(FloatArray &answer,
                              double sig,
                              double rho,
                              double tempKappa);

    /**
     * 1D: The mixed derivative of the plastic potential with respect
     * to the invariants and the hardening parameter are determined.
     */

    double computeDDGDInvDKappa1d(double sigma, double tempKappa);

    /**
     * For 3D: Computes the mixed derivative of the hardening parameter kappa with
     * respect to the plastic multiplier delta Lambda and the invariants sig
     * and rho.
     */
    void computeDDKappaDDeltaLambdaDInv(FloatArray &answer,
                                        double sig,
                                        double rho,
                                        double theta,
                                        double tempKappa);
    /**
     * For 1D: Computes the mixed derivative of the hardening parameter kappa with
     * respect to the plastic multiplier delta Lambda and the invariants sig
     * and rho.
     */
    double computeDDKappaDDeltaLambdaDInv1d(double sigma,
                                            double theta,
                                            double tempKappa);
    /**
     * Computes the derivative of the evolution law of the hardening parameter kappa with respect to the hardening variable kappa.
     */
    double computeDDKappaDDeltaLambdaDKappa(double sig, double rho, double theta, double tempKappa);

    /**
     * 1D: Computes the derivative of the evolution law of the hardening parameter kappa with respect to the hardening variable kappa.
     */
    double computeDDKappaDDeltaLambdaDKappa1d(double sig, double theta, double tempKappa);

    /**
     * Computes the derivative of the yield surface with respect to the
     * invariants sig and rho.
     */
    void computeDFDInv(FloatArray &answer,
                       double sig,
                       double rho,
                       double theta,
                       double tempKappa) const;

    /**
     * 1D: Computes the derivative of the yield surface with respect to the
     * invariants sig and rho.
     */
    double computeDFDInv1d(double sigma, double theta, double tempKappa) const;

    /**
     * Compute tempKappa.
     */
    double computeTempKappa(double kappaInitial,
                            double sigTrial,
                            double rhoTrial,
                            double sig);

    /**
     * Compute damage parameters
     */
    void  computeDamage(FloatArray &answer, const FloatArray &strain, const FloatMatrix &D, double timeFactor, GaussPoint *gp, TimeStep *tStep, double alpha, const FloatArray &effectiveStress);


    /**
     * Check for un- and reloading in the damage part
     */
    int checkForUnAndReloading(double &tempEquivStrain,
                               double &minEquivStrain,
                               const FloatMatrix &D,
                               GaussPoint *gp);

    double computeAlpha(FloatArray &effectiveStressTension, FloatArray &effectiveStressCompression, FloatArray &effectiveStress);

    /// Compute damage parameter in tension.
    virtual double computeDamageParamTension(double equivStrain, double kappaOne, double kappaTwo, double le, double omegaOld, double rateFactor);

    /// Compute damage parameter in compression.
    virtual double computeDamageParamCompression(double equivStrain, double kappaOne, double kappaTwo, double omegaOld, double rateFactor);

    /// Compute equivalent strain value for tension.
    double computeDeltaPlasticStrainNormTension(double tempKappaD, double kappaD, GaussPoint *gp);

    /// Compute equivalent strain value for compression.
    double computeDeltaPlasticStrainNormCompression(double tempAlpha, double tempKappaD, double kappaD, GaussPoint *gp, const double rho);

    /// Compute the base equivalent strain value.
    virtual double computeEquivalentStrain(double sig, double rho, double theta);

    /// Compute the ductility measure for the damage model.
    double computeDuctilityMeasureDamage(const FloatArray &strain, GaussPoint *gp, const double sig, const double rho);

    /**
     * Initialize the characteristic length, if damage is not yet activated
     * Set the increase factor for the strain rate dependence
     */
    void initDamaged(double kappa,
                     const FloatArray &strain,
                     GaussPoint *gp);

    /// Compute the Haigh-Westergaard coordinates.
    void computeCoordinates(const FloatArray &stress, double &sig, double &rho, double &theta);


    /// Assign state flag.
    void assignStateFlag(GaussPoint *gp);

    /// Computes the derivative of rho with respect to the stress.
    void computeDRhoDStress(FloatArray &answer,
                            const FloatArray &stress) const;

    /// Computes the derivative of sig with respect to the stress.
    void computeDSigDStress(FloatArray &answer) const;

    /// Computes the seconfd derivative of rho with the respect to the stress.
    void computeDDRhoDDStress(FloatMatrix &answer,
                              const FloatArray &stress) const;

    /// Computes the derivative of costheta with respect to the stress.
    void computeDCosThetaDStress(FloatArray &answer,
                                 const FloatArray &stress) const;

    /// Compute the derivative of R with respect to costheta.
    double computeDRDCosTheta(double theta, double ecc) const;

    FloatMatrixF<1,1> give1dStressStiffMtrx(MatResponseMode mode, GaussPoint *gp, TimeStep *tStep) const override;

    void give3dMaterialStiffnessMatrix(FloatMatrix &answer, MatResponseMode mode, GaussPoint *gp, TimeStep *tStep) override;

    /// Compute the 3d secant stiffness matrix.
    void compute3dSecantStiffness(FloatMatrix &answer, GaussPoint *gp, TimeStep *tStep);

    bool isCharacteristicMtrxSymmetric(MatResponseMode rMode) const override { return false; }

    int giveIPValue(FloatArray &answer, GaussPoint *gp, InternalStateType type, TimeStep *tStep) override;

protected:
    MaterialStatus *CreateStatus(GaussPoint *gp) const override;
};
} //end namespace oofem
#endif
