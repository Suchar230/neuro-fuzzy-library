/** @file */

#ifndef GRANULAR_DBSCAN_H
#define GRANULAR_DBSCAN_H

#include <memory>
#include <vector>

#include "../partitions/partitioner.h"
#include "../tnorms/t-norm.h"
#include "../snorms/s-norm.h"
#include "../descriptors/descriptor.h"
#include "../descriptors/descriptor-triangular.h"

namespace ksi
{

    /** The class implements GrDBSCAN clustering algorithm.
        @author Dawid Suchy
        @date 2022-03-11*/
    class granular_dbscan : virtual public partitioner
    {
    protected:
        /** Radius of a neighborhood */
        double _epsilon = -1;

        /** Minimum sum of points' membership values required to form a dense region */
        double _minPoints = -1;

        /** Maximum aggregated membership value allowed to consider the point as being a new core point */
        double _xi = -1;

        /** Minimum membership value required to consider the point as being core point's neighbour */
        double _psi = -1;

        /** Fuzzy clustering algorithm implementation */
        std::shared_ptr<partitioner> _pGranulationAlgorithm;

        /** S-norm */
        std::shared_ptr<s_norm> _pSnorm;

        /** T-norm */
        std::shared_ptr<t_norm> _pTnorm;

    public:
        granular_dbscan();

        granular_dbscan(
            const double epsilon,
            const double minPoints,
            const double xi,
            const double psi,
            const partitioner &granulationAlgorithm,
            const s_norm &snorm,
            const t_norm &tnorm);

        granular_dbscan(const granular_dbscan &obj);

        virtual partition doPartition(const dataset &ds) override;

        granular_dbscan &operator=(const granular_dbscan &obj);

        /** @return The method clones the object and returns the address of the copy. */
        virtual partitioner *clone() const override;

        virtual ~granular_dbscan();

        /** @return an abbreviation of a method */
        virtual std::string getAbbreviation() const override;

    protected:
        std::vector<std::vector<std::shared_ptr<descriptor>>> prepareGranularData(const dataset &ds, std::shared_ptr<partitioner> algorithm);

        std::size_t findMaxMembIndex(const std::size_t datasetSize, const std::vector<double> &memberships, const double psi, const std::vector<bool> &processed);

        std::vector<double> findNeighboursMemberships(const std::vector<std::vector<std::shared_ptr<descriptor>>> &granularDs, const std::vector<std::shared_ptr<descriptor>> &granule);

        const descriptor_triangular calculateDistance(
            const std::shared_ptr<descriptor> &firstDescriptor,
            const std::shared_ptr<descriptor> &secondDescriptor);

        const double calculateAreaPercentageInSpace(const double epsilon, const descriptor_triangular &triangle);

        /** @return The method returns the membership of data item d to granule granule.
         *  @param d data item
         *  @param granule set of descriptors
         *  @param pTnorm a tnorm
         *  @date   2022-09-23
         *  @author Krzysztof Siminski
         */
        double getMembershipToGranule(const datum &d, const std::vector<std::shared_ptr<descriptor>> &granule, const std::shared_ptr<t_norm> pTnorm);

        /** @return The method returns the membership matrix. Each row represents a cluster. Each column represents input data item. The i-th cell in the j-th column in the matrix holds membership value of the j-th input data item to the i-th cluster.
         *  @param ds dataset: a set of input data items
         *  @param granularDs granules: granulated dataset
         *  @param mU membership matrix for granules. Each row represents a cluster. Each column represents a granule.
         *  @date   2022-09-23
         *  @author Krzysztof Siminski
         */
        std::vector<std::vector<double>> getClusterDatumMatrix(const dataset &ds, const std::vector<std::vector<std::shared_ptr<descriptor>>> &granularDs, const std::vector<std::vector<double>> &mU);
    };
}
#endif
