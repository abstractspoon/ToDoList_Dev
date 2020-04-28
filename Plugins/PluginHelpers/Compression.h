#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;

////////////////////////////////////////////////////////////////////////////////////////////////

namespace Abstractspoon
{
	namespace Tdl
	{
		namespace PluginHelpers
		{
			public ref class Compression
			{
			public:
				static cli::array<Byte>^ Compress(cli::array<Byte>^ bytes);
				static cli::array<Byte>^ Decompress(cli::array<Byte>^ bytes);

				static cli::array<Byte>^ CompressString(String^ text);
				static String^ DecompressString(cli::array<Byte>^ bytes);
			};
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////

