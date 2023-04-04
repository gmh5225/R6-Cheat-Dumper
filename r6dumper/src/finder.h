#pragma once
#include "shared.h"
#include <thread>

namespace finder {
	void run() {
		std::ifstream file;
		file.open("offsets.db", std::ios::binary);

		std::string contents;

		contents.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

		while (contents.find(ENTRY_SEPERATOR) != std::string::npos) {
			size_t idx = contents.find(NAME_SEPERATOR);
			auto offset_name = contents.substr(0, idx);
			contents = contents.substr(idx + strlen(NAME_SEPERATOR));

			printf("\nsearching for offset %s\n", offset_name.c_str());

			if (contents.find(SIG_SEPERATOR) == 0) {
				contents = contents.substr(strlen(SIG_SEPERATOR));

				char* raw_data = const_cast<char*>(contents.c_str());
				raw_data;

				int64_t extra_offset = 0;
				if (contents.find(SIG_SEPERATOR) < contents.find(ENTRY_SEPERATOR))
					extra_offset = std::atoi(contents.substr(contents.find(SIG_SEPERATOR) + strlen(SIG_SEPERATOR), contents.find(SIG_SEPERATOR) - contents.find(ENTRY_SEPERATOR) - strlen(SIG_SEPERATOR)).c_str());

				std::vector<std::thread*> threads;
				threads.resize(THREAD_COUNT);



				for (int sig_position = 0; sig_position < 0x200; sig_position++) {
					for (uint64_t size = 0x10; sig_position + size < 0x200; size++) {
						uint8_t* data = new uint8_t[size];

						memcpy(data, raw_data + sig_position, size);

						auto matches = shared::sig_scan(data, size, segment::text);

						if (matches.empty())
							break;

						if (matches.size() == 1) {
							if (extra_offset != 0)
								printf("%s = 0x%" PRIX64 " - decrypt = 0x%" PRIX64 " \n", offset_name.c_str(), shared::extract_offset(matches[0] + 0x100 - sig_position + extra_offset), shared::extract_offset(matches[0] + 0x100 - sig_position));

							else
								printf("%s = 0x%" PRIX64 "\n", offset_name.c_str(), matches[0] + 0x100 - sig_position);


							sig_position = 1000;
							break;
						}

						delete data;
					}
				}
			}

			else {
				contents = contents.substr(strlen(STRING_SEPERATOR));

				while ((idx = contents.find(SEPERATOR)) < contents.find(ENTRY_SEPERATOR)) {

					idx = contents.find(SEPERATOR);

					auto str_to_find = contents.substr(0, idx);

					contents = contents.substr(idx + strlen(SEPERATOR));
					idx = contents.find(SEPERATOR);

					int64_t offset = std::atoi(contents.substr(0, idx).c_str());

					printf("search string %s, offset %d\n", str_to_find.c_str(), offset);

					char* cstr = const_cast<char*>(str_to_find.c_str());
					auto vec = shared::sig_scan(reinterpret_cast<uint8_t*>(cstr), str_to_find.length() + 1, segment::rdata);
					if (vec.empty()) {
						printf("error finding offset\n\n");
						continue;
					}
					uint64_t string_offset = vec.back();

					uint8_t rex = 0, instruction = 0, reg = 0;

					idx = contents.find(OPCODE_SEPERATOR);
					if (idx < contents.find(STRING_SEPERATOR) && idx < contents.find(ENTRY_SEPERATOR)) {
						contents = contents.substr(idx + strlen(OPCODE_SEPERATOR));

						rex = contents.c_str()[0] - '0';
						instruction = contents.c_str()[1] - '0';
						reg = contents.c_str()[2] - '0';
					}

					printf("found string at 0x%" PRIX64 "\n", string_offset);

					auto refs = shared::find_xrefs(string_offset, rex, instruction, reg);
					uint64_t xref;

					if (refs.size() == 1) {
						xref = shared::find_xrefs(string_offset).back();
						printf("found xref at 0x%" PRIX64 "\n", xref);

						xref -= offset;

						uint64_t address = shared::extract_relative_offset(xref);
						printf("found offset %s at 0x%" PRIX64 "\n\n", offset_name.c_str(), address);
					}

					// if there are multiple strings for this offset entry, move to the next one
					if (contents.find(STRING_SEPERATOR) < contents.find(ENTRY_SEPERATOR))
						contents = contents.substr(contents.find(STRING_SEPERATOR) + strlen(STRING_SEPERATOR));
				}
			}

			contents = contents.substr(contents.find(ENTRY_SEPERATOR) + strlen(ENTRY_SEPERATOR));
		}
	}
}