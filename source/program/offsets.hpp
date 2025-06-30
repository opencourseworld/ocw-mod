#pragma once

#include <common.hpp>
#include <tuple>

#include "version.hpp"

namespace exl::reloc {
    using VersionType = util::UserVersion;

    template<VersionType Version, impl::LookupEntry... Entries>
    using UserTableType = VersionedTable<Version, Entries...>;

    using UserTableSet = TableSet<VersionType,
        // This feature allows you to specify symbols in your executable to resolve with module+offset pairs.
        // They are packed up and sorted at compile time so they can be efficiently looked up.
        // The `exl::reloc::GetLookupTable` API is provided if you want to look up entries in the table explicitly.
        // Examples of tables:
    UserTableType<VersionType::DEFAULT,
    //    Module offset is relative to.     Offset within module.       Symbol name.
        { util::ModuleIndex::Main,
          0x482330,
          "_ZN3eui10MessageMgr7Archive4loadEPN4sead4HeapEPvj"
        },
        { util::ModuleIndex::Main,
          0x482990,
          "_ZN3eui10MessageSet11findMessageEPKc"
        },
        { util::ModuleIndex::Main,
          0x482A20,
          "_ZN3eui10MessageSet14tryFindMessageEPKc"
        },
        { util::ModuleIndex::Main,
          0xE7F130,
          "_ZN4Cstm9UIMgrCstm11initMessageEPN2Lp2UI10MessageMgrE"
        }
    >,

    UserTableType<VersionType::V3_0_2,
    //    Module offset is relative to.     Offset within module.       Symbol name.
        { util::ModuleIndex::Main,
          0x482340,
          "_ZN3eui10MessageMgr7Archive4loadEPN4sead4HeapEPvj"
        },
        { util::ModuleIndex::Main,
          0x4829A0,
          "_ZNK3eui10MessageSet11findMessageEPKc"
        },
        { util::ModuleIndex::Main,
          0x482A30,
          "_ZNK3eui10MessageSet14tryFindMessageEPKc"
        },
        { util::ModuleIndex::Main,
          0xE7F140,
          "_ZN4Cstm9UIMgrCstm11initMessageEPN2Lp2UI10MessageMgrE"
        }
    >

        // In addition, you can specify multiple tables and select the correct one at runtime. This allows you to
        // support multiple versions/variations of a game in one executable. See version.hpp to see how to implement
        // multiple supported versions.
        // UserTableType<VersionType::OTHER,
        // //    Module offset is relative to.     Offset within module.       Symbol name.
        //     { util::ModuleIndex::Main,          0x4201,                     "example1" },
        //     { util::ModuleIndex::Sdk,           0x4202,                     "example2" }
        // >
    >;
}
