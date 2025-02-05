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
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef latticeplasticitydamage_h
#define latticeplasticitydamage_h

#include "latticelinearelastic.h"
#include "latticematstatus.h"

///@name Input fields for LatticePlasticityDamage
//@{
#define _IFT_LatticePlasticityDamage_Name "latticeplastdam"
#define _IFT_LatticePlasticityDamage_tol "tol"
#define _IFT_LatticePlasticityDamage_iter "iter"
#define _IFT_LatticePlasticityDamage_sub "sub"
#define _IFT_LatticePlasticityDamage_ft "ft"
#define _IFT_LatticePlasticityDamage_fc "fc"
#define _IFT_LatticePlasticityDamage_angle1 "angle1"
#define _IFT_LatticePlasticityDamage_angle2 "angle2"
#define _IFT_LatticePlasticityDamage_flow "flow"
#define _IFT_LatticePlasticityDamage_stype "stype"
#define _IFT_LatticePlasticityDamage_wf "wf"
#define _IFT_LatticePlasticityDamage_ft1 "ft1"
#define _IFT_LatticePlasticityDamage_wf1 "wf1"
#define _IFT_LatticePlasticityDamage_ahard "ahard"
#define _IFT_LatticePlasticityDamage_damage "damage"
//@}

namespace oofem {
/**
 * This class implements associated Material Status to LatticePlasticityDamage.
 * @author: Peter Grassl
 */
class LatticePlasticityDamageStatus : public LatticeMaterialStatus
{
protected:

    FloatArray elasticStrain;

    FloatArray tempElasticStrain;

    double kappaP = 0.;

    double tempKappaP = 0.;

    double kappaDOne = 0., kappaDTwo = 0.;//, kappaDThree;

    double tempKappaDOne = 0., tempKappaDTwo = 0.;//, tempKappaDThree;

    double damage = 0.;

    double tempDamage = 0.;

    //double e0;

    int compressionFlag = 0;

public:

    /// Constructor
    LatticePlasticityDamageStatus(int n, Domain *d, GaussPoint *g);

    double giveKappaP() const { return kappaP; }

    double giveTempKappaP() const { return tempKappaP; }

    double giveKappaDOne() const { return kappaDOne; }
    double giveKappaDTwo() const { return kappaDTwo; }
    //   double giveKappaDThree() { return kappaDThree; }
    double giveTempKappaDOne() const { return tempKappaDOne; }
    double giveTempKappaDTwo() const { return tempKappaDTwo; }
    //double giveTempKappaDThree() { return tempKappaDThree; }

    void  giveTempElasticStrain(FloatArray &answer) const
    { answer = tempElasticStrain; }

    void  giveElasticStrain(FloatArray &answer) const
    { answer = elasticStrain; }

    void  letTempPlasticStrainBe(const FloatArray &v)
    { tempPlasticStrain = v; }

    void  letTempElasticStrainBe(const FloatArray &v)
    { tempElasticStrain = v; }

    void   setTempKappaP(double newKappa) { tempKappaP = newKappa; }

    void   setTempKappaDOne(double newKappa) { tempKappaDOne = newKappa; }

    void   setTempKappaDTwo(double newKappa) { tempKappaDTwo = newKappa; }

    //void   setTempKappaDThree(double newKappa) { tempKappaDThree = newKappa; }

    double giveDamage() const { return damage; }

    double giveTempDamage() const { return tempDamage; }

    void   setTempDamage(double newDamage) { tempDamage = newDamage; }

    int giveCompressionFlag() const { return compressionFlag; }

    void setCompressionFlag(int flag) { compressionFlag = flag; }

    void printOutputAt(FILE *file, TimeStep *tStep) const override;

    const char *giveClassName() const override { return "LatticePlasticityDamageStatus"; }

    void initTempStatus() override;

    void updateYourself(TimeStep *) override;

    void saveContext(DataStream &stream, ContextMode mode) override;

    void restoreContext(DataStream &stream, ContextMode mode) override;
};

/**
 * This class implements a local random plasticity damage model for concrete for lattice elements.
 */
class LatticePlasticityDamage : public LatticeLinearElastic
{
protected:

    enum LatticePlasticityDamage_ReturnResult { RR_NotConverged, RR_Converged };
    LatticePlasticityDamage_ReturnResult returnResult = RR_NotConverged;

    double initialYieldStress = 0.;

    /// tensile strength
    double ft = 0.;
    /// compressive strength
    double fc = 0.;
    /// frictional angle of the yield surface
    double frictionAngleOne = 0.;
    /// frictional angle of the yield surface
    double frictionAngleTwo = 0.;
    /// frictional angle of the plastic potential
    double flowAngleOne = 0.;

    /// frictional angle of the plastic potential
    double flowAngleTwo = 0.;

    /// determines the softening -> corresponds to crack opening (not strain) when tension stress vanishes
    double wf = 0.;

    /// softening type determines the type of softening. 0 is exponential and 1 is bilinear.
    int softeningType = 0;

    /// tensile stress value for bilinear stress-crack opening curve
    double ftOne = 0.;

    /// crack opening value for bilinear stress-crack opening curve
    double wfOne = 0.;

    ///hardening parameter
    double aHard = 0.;

    /// first parameter for thermal expansion
    //double tAlphaOne;
    /// second parameter for thermal expansion
    //double tAlphaTwo;
    /// third parameter for thermal expansion
    //double tAlphaThree;

    /// yield tolerance
    double yieldTol = 0.;
    /// maximum number of iterations for stress return
    int newtonIter = 0;
    int numberOfSubIncrements = 0;

    //damageFlag
    int damageFlag = 0;

public:

    /// Constructor
    LatticePlasticityDamage(int n, Domain *d);

    const char *giveInputRecordName() const override { return _IFT_LatticePlasticityDamage_Name; }
    const char *giveClassName() const override { return "LatticePlasticityDamage"; }


    void initializeFrom(InputRecord &ir) override;

    bool isCharacteristicMtrxSymmetric(MatResponseMode rMode) const override { return false; }

    FloatMatrixF< 6, 6 >give3dLatticeStiffnessMatrix(MatResponseMode rMode, GaussPoint *gp, TimeStep *tStep) const override;

    bool hasMaterialModeCapability(MaterialMode mode) const override;

    void computeBMatrix(FloatMatrix &answer,
                        const FloatArray &sigma,
                        const double deltaLambda);

    void computeAMatrix(FloatMatrix &answer,
                        const FloatArray &sigma,
                        const double tempKappa,
                        const double deltaLambda,
                        GaussPoint *gp);

    void computeFVector(FloatArray &answer,
                        const FloatArray &sigma,
                        const double deltaLambda,
                        GaussPoint *gp);

    void computeMVector(FloatArray &answer,
                        const FloatArray &sigma,
                        const double deltaLambda,
                        GaussPoint *gp);

    void computeDMMatrix(FloatMatrix &answer,
                         const FloatArray &sigma,
                         const double deltaLambda,
                         GaussPoint *gp);


    void computeJacobian(FloatMatrix &answer,
                         const FloatArray &sigma,
                         const double tempKappa,
                         const double deltaLambda,
                         GaussPoint *gp);

    int computeInverseOfJacobian(FloatMatrix &answer,
                                 const FloatMatrix &src);


    virtual void computeDamageParam(double &omega, double kappaOne, double kappaTwo, GaussPoint *gp);

    FloatArrayF< 6 >giveLatticeStress3d(const FloatArrayF< 6 > &jump, GaussPoint *gp, TimeStep *tStep) override;

    void performPlasticityReturn(FloatArray &stress,
                                 GaussPoint *gp,
                                 const FloatArray &totalStrain,
                                 TimeStep *tStep);

    void performDamageEvaluation(GaussPoint *gp,
                                 FloatArray &reducedStrain);

    double performRegularReturn(FloatArray &stress, double yieldValue, GaussPoint *gp);

    double computeYieldValue(const FloatArray &sigma,
                             const double tempKappa,
                             GaussPoint *gp);

    double computeHardening(const double kappa,
                            GaussPoint *gp);


    double computeDHardeningDKappa(const double kappa,
                                   GaussPoint *gp);
    double computeDDHardeningDDKappa(const double kappa,
                                     GaussPoint *gp);

    double computeDuctilityMeasure(FloatArray &stress, double ductilityParameter);

    double computeYieldStress(double kappaP,
                              GaussPoint *gp);

    double computeEquivalentStress(const FloatArray &tempSigma);

    MaterialStatus *CreateStatus(GaussPoint *gp) const override;

    virtual void giveReducedStrain(FloatArray &answer, GaussPoint *gp, TimeStep *tStep);


protected:

    int giveIPValue(FloatArray &answer,
                    GaussPoint *gp,
                    InternalStateType type,
                    TimeStep *atTime) override;

    int giveIPValueSize(InternalStateType type,
                        GaussPoint *gp);

    int giveIntVarCompFullIndx(IntArray &answer,
                               InternalStateType type,
                               MaterialMode mmode);

    InternalStateValueType giveIPValueType(InternalStateType type);
};
} // end namespace oofem

#endif
