#include <filedevice/seadArchiveFileDevice.h>
#include <resource/seadSharcArchiveRes.h>

struct LMSMessage;

namespace sead {

struct MessageSetBase {
  virtual ~MessageSetBase();
  bool initialize(void *, sead::Heap *);
  LMSMessage *m_pLMSMessage;
  int m_TextNum;
};

} // namespace sead

namespace eui {

struct MessageString {
  MessageString(int strLen, char16_t const *pStr)
      : m_pStr(pStr), m_StrLen(strLen) {}
  MessageString(MessageString &&) = delete;
  MessageString(const MessageString &) = delete;
  MessageString &operator=(MessageString &&) = delete;
  MessageString &operator=(const MessageString &) = delete;
  ~MessageString() = default;
  char16_t const *m_pStr;
  int m_StrLen;
};

struct SharcArchive {
  struct FileReader {
    ~FileReader();
    bool readNext();
    sead::ArchiveFileDevice m_ArchiveFileDevice;
    int m_Index;
    sead::DirectoryHandle m_DirectoryHandle;
    sead::DirectoryEntry m_DirectoryEntry;
  };
  void initialize(sead::Heap *, void *, unsigned int);
  void startFileReader(FileReader *) const;
  sead::SharcArchiveRes *m_pSharcArchiveRes;
};

struct MessageSet : sead::MessageSetBase {
  MessageSet();
  virtual ~MessageSet();
  MessageString findMessage(char const *);
  MessageString tryFindMessage(char const *);
};

struct MessageMgr {
  struct Archive : sead::IDisposer {
    void load(sead::Heap *, void *, unsigned int);
    sead::ListNode m_ListNode;
    SharcArchive m_SharcArchive;
    sead::Buffer<MessageSet> m_MessageSets;
    MessageMgr *m_pMessageMgr;
    void *m_pDataBuffer;
  };
};

} // namespace eui

static eui::MessageSet *s_pSourceMessageSet = nullptr;
static eui::MessageSet *s_pDestinationMessageSet = nullptr;

HOOK_DEFINE_REPLACE(EuiMessageMgrArchiveLoad) {
  static void Callback(eui::MessageMgr::Archive *pArchive, sead::Heap *pHeap,
                       void *pDataBuffer, unsigned int dataBufferSize) {
    pArchive->m_pDataBuffer = pDataBuffer;
    pArchive->m_SharcArchive.initialize(pHeap, pDataBuffer, dataBufferSize);
    pArchive->m_MessageSets.allocBufferAssert(
        pArchive->m_SharcArchive.m_pSharcArchiveRes->mFATEntrys.size(), pHeap);
    eui::SharcArchive::FileReader fileReader{sead::ArchiveFileDevice(nullptr),
                                             -1, sead::DirectoryHandle(),
                                             sead::DirectoryEntry()};
    pArchive->m_SharcArchive.startFileReader(&fileReader);
    while (fileReader.readNext()) {
      if (fileReader.m_DirectoryEntry.name.include(".msbt")) {
        auto &messageSet = pArchive->m_MessageSets[fileReader.m_Index];
        messageSet.initialize(const_cast<void *>(fileReader.m_ArchiveFileDevice.mArchive->getFileFast(
            fileReader.m_Index, nullptr)), pHeap);
        if (fileReader.m_DirectoryEntry.name == "Cmn_TagWindow_00.msbt") {
          s_pSourceMessageSet = &messageSet;
        } else if (fileReader.m_DirectoryEntry.name == "OpenCourseWorld_TagWindow_00.msbt") {
          s_pDestinationMessageSet = &messageSet;
        }
      }
    }
  }
};

namespace Lp {

namespace UI {

struct MessageMgr {
  void loadResMgrStaticHeap(sead::SafeString const &, bool);
};

} // namespace UI

} // namespace Lp

namespace Cstm {

struct UIMgrCstm {
  void initMessage(Lp::UI::MessageMgr *);
};

} // namespace Cstm

HOOK_DEFINE_TRAMPOLINE(EuiMessageSetFindMessage) {
  static eui::MessageString Callback(eui::MessageSet *pMessageSet,
                                     char const *pLabel) {
    if (s_pSourceMessageSet && pMessageSet == s_pSourceMessageSet) {
      pMessageSet = s_pDestinationMessageSet;
    }
    return Orig(pMessageSet, pLabel);
  }
};

HOOK_DEFINE_TRAMPOLINE(EuiMessageSetTryFindMessage) {
  static eui::MessageString Callback(eui::MessageSet *pMessageSet,
                                     char const *pLabel) {
    if (s_pSourceMessageSet && pMessageSet == s_pSourceMessageSet) {
      pMessageSet = s_pDestinationMessageSet;
    }
    return Orig(pMessageSet, pLabel);
  }
};

HOOK_DEFINE_TRAMPOLINE(CstmUIMgrCstmInitMessage) {
  static void Callback(Cstm::UIMgrCstm *pUIMgrCstm,
                       Lp::UI::MessageMgr *pMessageMgr) {
    Orig(pUIMgrCstm, pMessageMgr);
    pMessageMgr->loadResMgrStaticHeap("OpenCourseWorldMsg", false);
  }
};

static inline void install_ui(void) {
  EuiMessageMgrArchiveLoad::InstallAtFuncPtr(&eui::MessageMgr::Archive::load);
  EuiMessageSetFindMessage::InstallAtFuncPtr(&eui::MessageSet::findMessage);
  EuiMessageSetTryFindMessage::InstallAtFuncPtr(&eui::MessageSet::tryFindMessage);
  CstmUIMgrCstmInitMessage::InstallAtFuncPtr(&Cstm::UIMgrCstm::initMessage);
}
