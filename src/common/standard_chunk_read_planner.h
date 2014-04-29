#pragma once

#include "common/read_planner.h"

class StandardChunkReadPlanner : public ReadPlanner {
public:
	virtual void prepare(const std::vector<ChunkType>& availableParts,
			const std::map<ChunkType, float>& serverScores);
	virtual std::vector<ChunkType> partsToUse() const;
	virtual bool isReadingPossible() const;
	virtual Plan buildPlanFor(uint32_t firstBlock, uint32_t blockCount) const;

private:
	enum PlanBuilderType {
		BUILDER_NONE,
		BUILDER_STANDARD,
		BUILDER_XOR,
	};

	class PlanBuilder {
	public:
		PlanBuilder(PlanBuilderType type) : type_(type) {}
		virtual ~PlanBuilder() {}
		virtual Plan buildPlan(uint32_t firstBlock, uint32_t blockCount) const = 0;
		PlanBuilderType type() const { return type_; }

	private:
		const PlanBuilderType type_;
	};

	class StandardPlanBuilder;
	class XorPlanBuilder;

	std::map<PlanBuilderType, std::unique_ptr<PlanBuilder>> planBuilders_;
	PlanBuilder* currentBuilder_;

	void setCurrentBuilderToStandard();
	void setCurrentBuilderToXor(ChunkType::XorLevel level, ChunkType::XorPart missingPart);
	void unsetCurrentBuilder();
};