#ifndef TS_HPP_INCLUDE_GUARD_48824554
#define TS_HPP_INCLUDE_GUARD_48824554

#include <string>
#include "UID.hpp"
namespace dicom
{

	//!Allows quick access to attributes of a given transfer syntax.
	/*!
		Part 5, Section 10 describes Transfer Syntaxes.
	*/
	class TS//don't like this name. - we should rename it "TransferSyntax", (as distinct from primitive::TransferSyntax !)
	{

	public:

		TS(const UID& uid);

		bool isExplicitVR() const;
		bool isBigEndian() const;
		bool isDeflated() const;
		bool isEncapsulated() const;

		UID getUID() const;
	private:
		const UID uid_;
	};
}//namespace dicom
#endif //TS_HPP_INCLUDE_GUARD_48824554
