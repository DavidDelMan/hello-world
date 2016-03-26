#include "SuperSobolIndices.h"

/* Ctor
 * Input:
 *
 * model_ = Type-valued function of a vector of Types.  First arg is
 *   vector of parameters to be drawn randomly.  Second arg is vector
 *   of fixed constants, like strike & interest rate.
 * constants_ = vector of constants for model, like strike price
 * indices_ = set of parameter indices to compute SIs for
 * paramUncertaintyDistroParams_ = "hyperparameters" giving the 
 *    parameters for the model parameters' uncertainties distros
 * dim_ = number of model parameters
 * N_MC_ = number of Monte Carlo runs to compute Sobol indices
 * N_Super_Sobol_ = number of Monte Carlo runs to compute Super Sobol
 *    indices
 */
SuperSobolIndices::
SuperSobolIndices(Type (*model_)(const std::vector<Type>&, 
				 const std::vector<Type>&),
		  const std::vector<Type>& constants_,
		  const std::set<int>& indices_,
		  const std::vector<std::vector<Type> >& 
		  paramUncertaintyDistroParams_,
		  const unsigned int dim_,
		  const unsigned int N_MC_,
		  const unsigned int N_Super_Sobol_)
{
  model = model_;
  constants = constants_;
  indices = indices_;
  paramUncertaintyDistroParams = paramUncertaintyDistroParams_;
  dim = dim_;
  N_MC = N_MC_;

  // intialize Super Sobol indices
  lowerSuperIndex = 0;
  totalSuperIndex = 0;

  // allocate model argument vectors
  s1.resize(dim);
  s2.resize(dim);
  s_arg1.resize(dim);
  s_arg2.resize(dim);

  // construct halton (RASRAP) & InverseTransformation objects
  RNG = new halton();
  invTrans = new InverseTransformation();

  // init RNG: length of Halton vector, random start, random permute
  RNG->init(2*dim,true,true);
}

void SuperSobolIndices::
ComputeSuperSobolIndices()
{
  // MC accumulators
  Type f0_sum_super = 0, Dy_sum_super = 0, DT_sum_super = 0, 
    D_sum_super = 0;

  // model evaluations
  Type f, f2, model1, model2;

  for (unsigned int i = 0; i < N_MC; ++i)
    {
      // generate 2*dim random numbers
      RNG->genHalton();

      // transform each random number to parameter uncertainty distro
      TransformToParamUncertaintyDomain();

      // assign xformed RVs to proper model argument vectors
      AssignUncertaintyModelArguments();

    }
}

/* Fills the s_arg1 and s_arg2 member vectors that hold the 
 * uncertainties for the corresponding parameters according to the
 * parameter index for which we are computing Super Sobol indices for
 */
void SuperSobolIndices::
AssignUncertaintyModelArguments()
{
  for (int j = 0; j < dim; ++j)
    {
      // check if "j" is in index set to compute Super Sobol index for
      bool inIndexSet = indices.count(j+1);

      if (inIndexSet)
	{
	  s_arg1[j] = s1[j];
	  s_arg2[j] = s2[j];
	}
      else
	{
	  s_arg1[j] = s2[j];
	  s_arg2[j] = s1[j];
	}
    }
}

/* Retrieves the Unif(0,1) random numbers generates in 
 * ComputeSuperSobolIndices() and transforms them to the respective
 * distributions of the uncertainties, s1 and s2.
 */
void SuperSobolIndices::
TransformToParamUncertaintyDomain()
{
  // retrieve random numbers computed in ComputeSuperSobolIndices()
  for (int j = 0; j < dim; ++j)
    {
      s1[j] = RNG->get_rnd(j+1);
      s2[j] = RNG->get_rnd(j+1+dim);

      // left and right endpoints of uniform uncertainty distros
      Type a = paramUncertaintyDistroParams[j][0];
      Type b = paramUncertaintyDistroParams[j][0];

      // generate Unif(a,b) RVs from uncertainties
      s1[j] = invTrans->Uniform(s1[j],a,b);
      s2[j] = invTrans->Uniform(s2[j],a,b);
    }
}
