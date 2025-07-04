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
        },
        { util::ModuleIndex::Main,
          0x485190,
          "_ZN3eui12SharcArchive10initializeEPN4sead4HeapEPvj"
        },
        { util::ModuleIndex::Main,
          0x262BB0,
          "_ZnamPN4sead4HeapEiRKSt9nothrow_t"
        },
        { util::ModuleIndex::Main,
          0x482970,
          "_ZN3eui10MessageSetC1Ev"
        },
        { util::ModuleIndex::Main,
          0x1CB4300,
          "_ZN4sead17ArchiveFileDeviceC1EPNS_10ArchiveResE"
        },
        { util::ModuleIndex::Main,
          0x485330,
          "_ZNK3eui12SharcArchive15startFileReaderEPNS0_10FileReaderE"
        },
        { util::ModuleIndex::Main,
          0x4853F0,
          "_ZN3eui12SharcArchive10FileReader8readNextEv"
        },
        { util::ModuleIndex::Main,
          0x2A99C0,
          "_ZN4sead14MessageSetBase10initializeEPvPNS_4HeapE"
        },
        { util::ModuleIndex::Main,
          0x485390,
          "_ZN3eui12SharcArchive10FileReaderD1Ev"
        },
        { util::ModuleIndex::Main,
          0x758480,
          "_ZN2Lp2UI10MessageMgr20loadResMgrStaticHeapERKN4sead14SafeStringBaseIcEEb"
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
          "_ZN3eui10MessageSet11findMessageEPKc"
        },
        { util::ModuleIndex::Main,
          0x482A30,
          "_ZN3eui10MessageSet14tryFindMessageEPKc"
        },
        { util::ModuleIndex::Main,
          0xE7F0F0,
          "_ZN4Cstm9UIMgrCstm11initMessageEPN2Lp2UI10MessageMgrE"
        },
        { util::ModuleIndex::Main,
          0x4851A0,
          "_ZN3eui12SharcArchive10initializeEPN4sead4HeapEPvj"
        },
        { util::ModuleIndex::Main,
          0x262BC0,
          "_ZnamPN4sead4HeapEiRKSt9nothrow_t"
        },
        { util::ModuleIndex::Main,
          0x482980,
          "_ZN3eui10MessageSetC1Ev"
        },
        { util::ModuleIndex::Main,
          0x1CB42C0,
          "_ZN4sead17ArchiveFileDeviceC1EPNS_10ArchiveResE"
        },
        { util::ModuleIndex::Main,
          0x485340,
          "_ZNK3eui12SharcArchive15startFileReaderEPNS0_10FileReaderE"
        },
        { util::ModuleIndex::Main,
          0x485400,
          "_ZN3eui12SharcArchive10FileReader8readNextEv"
        },
        { util::ModuleIndex::Main,
          0x2A99D0,
          "_ZN4sead14MessageSetBase10initializeEPvPNS_4HeapE"
        },
        { util::ModuleIndex::Main,
          0x485400,
          "_ZN3eui12SharcArchive10FileReaderD1Ev"
        },
        { util::ModuleIndex::Main,
          0x758490,
          "_ZN2Lp2UI10MessageMgr20loadResMgrStaticHeapERKN4sead14SafeStringBaseIcEEb"
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
