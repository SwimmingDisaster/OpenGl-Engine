#include "yaml-cpp/node/node.h"
#include "yaml-cpp/nodebuilder.h"
#include "yaml-cpp/nodeevents.h"

namespace YAML {
Node Clone(const Node& node) {
	NodeEvents events(node);
	NodeBuilder builder;
	events.Emit(builder);
	return builder.Root();
}
}  // namespace YAML
