/**
 *	@file 	relation_infra.hh
 *	@author	Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de)
 *	@brief	Serves as simple infrastructure for the class 'Relation' (@see relation.hh)
 *	@bugs 	Currently no bugs known
 *
 *	@section DESCRIPTION
 *	This file provides two structs which contain the meta information an attribute and a container have.
 */

#ifndef RELATION_INFRA_HH
#define RELATION_INFRA_HH

#include "infra/webe/types.hh"

#include <string>
#include <vector>

/**
 *	Each attribute of a relation has several properties, these properties are described by this struct (Attribute Description Type) 
 */
struct attr_desc_t
{
	/* the name of the attribute */
	std::string _attrName;
	/* the type of the attribute (e.g., char, integer, and so on... @see infra/webe/types.hh for the different types). Types are represented by enums */
	SchemaTypes_et _attrType;
	/* the index of the attribute within the logical schema of the relation (e.g., index of 3 indicates that the attribute is the 4th attribute in the schema) */
	uint _logSchemaIndex;
	/* a relation stores several data types within containers (e.g., c-strings and c++ strings). This variable indicates the type of container the attribute has (@see infra/webe/types.hh for the different types). Types are represented by enums */
	ContainerTypes_et _containerType;
	/* the attribute's container index (e.g., index of 3 indicates that the attribute is stored in the 4th container */
	int _containerIndex;

	const std::string 		attrName() const { return _attrName; }
	const SchemaTypes_et 	attrType() const { return _attrType; }
	const uint 				logSchemaIndex() const { return _logSchemaIndex; }
	const ContainerTypes_et	containerType() const { return _containerType; }
	const int 				containerIndex() const { return _containerIndex; }

};
typedef std::vector<attr_desc_t> attr_desc_vt;

/**
 *	A relation stores several data types within containers, the properties of a container are described by this struct (Container Description Type) 
 */
struct cont_desc_t
{
	/* the type of the container (e.g., simple char container, dictionary, and so on... @see infra/moer/simplecharcontainer.hh or @see infra/moer/simpledictionary.hh for details) */
	ContainerTypes_et _containerType;
	/* a helper variable depending on the container type: if container type is SimpleCharContainer this variable indicates the chunk size, if container is a dictionary it indicates the attr index */
	uint _helper;

	const ContainerTypes_et containerType() const {return _containerType;}
	const uint helper() const {return _helper;}
};
typedef std::vector<cont_desc_t> cont_desc_vt;

#endif