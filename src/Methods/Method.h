#pragma once

#include "../EventListener.h"
#include "../Grids/Grid.h"
#include <boost/mpi.hpp>

// Forward declare.
namespace Json {
	class Value;
}

namespace SSAGES
{
	//! Interface for Method implementations.
	/*!
	 * This class is designed to be the base class on which all Metadynamics
	 * methods are based.
	 *
	 * \ingroup Methods
	 */
	class Method : public EventListener, public Serializable
	{
	protected:
		boost::mpi::communicator _world; //!< Global MPI communicator
		boost::mpi::communicator _comm; //!< Local MPI communicator

		//! Pointer to grid
		Grid* _grid;

		//! Number of the method iteration.
		int _iteration;

	public:
		//! Constructor
		/*!
		 * \param frequency Frequency of sampling.
		 * \param world MPI world communicator.
		 * \param comm MPI local communicator.
		 *
		 * Frequency of sampling must be specified by all methods.
		 */
		Method(unsigned int frequency, 
			boost::mpi::communicator& world, 
			boost::mpi::communicator& comm) : 
		EventListener(frequency), _world(world), _comm(comm),
		_grid(nullptr), _iteration(0){}

		//! Set Method's iteration.
		/*!
		 * \param iter int value for what the method iteration should be.
		 */
		void SetIteration(int iter) {_iteration = iter;}

		//! Get Method's iteration.
		int GetIteration() {return _iteration;}

		//! Method call prior to simulation initiation.
		/*!
		 * \param snapshot Pointer to the simulation snapshot.
		 * \param cvs List of CVs.
		 *
		 * This function will be called before the simulation is started.
		 */
		virtual void PreSimulation(Snapshot* snapshot, const CVList& cvs) override = 0;

		//! Method call post integration.
		/*!
		 * \param snapshot Pointer to the simulation snapshot.
		 * \param cvs List of CVs.
		 *
		 * This function will be called after each integration step.
		 */
		virtual void PostIntegration(Snapshot* snapshot, const CVList& cvs) override = 0;

		//! Method call post simulation.
		/*!
		 * \param snapshot Pointer to the simulation snapshot.
		 * \param cvs List of CVs.
		 *
		 * This function will be called after the end of the simulation run.
		 */
		virtual void PostSimulation(Snapshot* snapshot, const CVList& cvs) override = 0;

		//! Set up the Grid
		/*!
		 * \param json JSON Value containing the input information
		 * \param path Path for JSON path specification.
		 */
		void BuildGrid(const Json::Value& json, const std::string& path)
		{
			_grid = Grid::BuildGrid(json, path);
		}

		//! Get the Grid
		Grid* GetGrid() const {return _grid;}

		//! Set up the Method
		/*!
		 * \param json JSON Value containing all input information.
		 * \param world MPI global communicator.
		 * \param comm MPI local communicator.
		 * \param path Path for JSON path specification.
		 * \return Pointer to the Method built. nullptr if an unknown error occurred.
		 *
		 * This function builds a method from a JSON node. It will return a
		 * nullptr when an unknown error occurred, but generally it will throw
		 * a BuildException on failure.
		 *
		 * \note Object lifetime is the caller's responsibility.
		 */
		static Method* BuildMethod(const Json::Value& json,
								boost::mpi::communicator& world, 
								boost::mpi::communicator& comm,
							   	const std::string& path);

		//! Destructor
		virtual ~Method() 
		{
			delete _grid;
		}
	};
}