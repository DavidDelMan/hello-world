#ifndef SUPERSOBOLINDICES_H
#define SUPERSOBOLINDICES_H

typedef double Type;

class SuperSobolIndices
{
 private:
  SobolIndices *sobol;  // sobol indices object; computes S's
  halton *RNG;  // random number generator = RASRAP
  InverseTransformation *invTrans;  // inverse transformation object
  Type (*model)(const std::vector<Type>&,
		const std::vector<Type>&);  // model

  // distribution of parameter uncertainties
  std::vector<std::vector<Type> > paramUncertaintyDistroParams;

  // number of MC runs to compute Super Sobol indices
  unsigned int N_Super_Sobol;
  unsigned int N_MC;  // number of MC runs to compute Sobol indices
  std::set<int> indices;  // index set to compute Super Sobol index of
  std::vector<Type> constants;  // model constants, if needed
  Type lowerSuperIndex, totalSuperIndex;  // Super Sobol indices

  /* Each element contains the uncertainty for the corresponding
   * parameter on a given run.  E.g. assume a four-parameter model with
   * parameters X1, X2, X3, X4.  To compute the Sobol indices we
   * generate two realizations (x1,x2,x3,x4) and (z1,z2,z3,z4), and for
   * each element of either vector we need to specify the uncertainty
   * (variance) of it.
   * For a given iteration in ComputeSuperSobolIndices():
   *     s1[0] = variance(x1),  s2[0] = variance(z1),
   *     s2[1] = variance(x2),  s2[1] = variance(z2),
   *     s3[2] = variance(x3),  s2[2] = variance(z3),
   *     s4[3] = variance(x4),  s2[3] = variance(z4).
   * These variances (s1 and s2) are then assigned to s_arg1 and s_arg2
   * depending on the parameter index for which we are computing the
   * Sobol index (this is done in AssignUncertaintyModelArguments()).
*/
  std::vector<Type> s1, s2, s_arg1, s_arg2;


 public:
  SuperSobolIndices(Type (*model_)(const std::vector<Type>&, 
				   const std::vector<Type>&),
		    const std::vector<Type>& constants_,
		    const std::set<int>& indices_,
		    const std::vector<std::vector<Type> >& 
		    paramUncertaintyDistroParams_,
		    const unsigned int dim_,
		    const unsigned int N_MC_,
		    const unsigned int N_Super_Sobol_);
  void ComputeSuperSobolIndices();
  void TransformToParamUncertaintyDomain();

  ~SuperSobolIndices()
    {
      delete RNG;
      delete InvTrans;
      delete sobol;
    }

};
#endif
