// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// _DRecipientControl wrapper class

namespace OutlookAPI
{

	class _Item : public COleDispatchDriver
	{
	public:
		_Item() {}		// Calls COleDispatchDriver default constructor
		_Item(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
		_Item(const _Item& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	public:
		// Common Properties
		LPDISPATCH GetApplication();
		long GetClass();
		LPDISPATCH GetSession();
		LPDISPATCH GetParent();
		LPDISPATCH GetActions();
		LPDISPATCH GetAttachments();
		CString GetBillingInformation();
		void SetBillingInformation(LPCTSTR lpszNewValue);
		CString GetBody();
		void SetBody(LPCTSTR lpszNewValue);
		CString GetCategories();
		void SetCategories(LPCTSTR lpszNewValue);
		CString GetCompanies();
		void SetCompanies(LPCTSTR lpszNewValue);
		CString GetConversationIndex();
		CString GetConversationTopic();
		DATE GetCreationTime();
		CString GetEntryID();
		LPDISPATCH GetFormDescription();
		LPDISPATCH GetGetInspector();
		long GetImportance();
		void SetImportance(long nNewValue);
		DATE GetLastModificationTime();
		CString GetMessageClass();
		void SetMessageClass(LPCTSTR lpszNewValue);
		CString GetMileage();
		void SetMileage(LPCTSTR lpszNewValue);
		BOOL GetNoAging();
		void SetNoAging(BOOL bNewValue);
		long GetOutlookInternalVersion();
		CString GetOutlookVersion();
		BOOL GetSaved();
		long GetSensitivity();
		void SetSensitivity(long nNewValue);
		long GetSize();
		CString GetSubject();
		void SetSubject(LPCTSTR lpszNewValue);
		BOOL GetUnRead();
		void SetUnRead(BOOL bNewValue);
		LPDISPATCH GetUserProperties();
		void Close(long SaveMode);
		LPDISPATCH Copy();
		void Delete();
		void Display(const VARIANT& Modal);
		LPDISPATCH Move(LPDISPATCH DestFldr);
		void PrintOut();
		void Save();
		void SaveAs(LPCTSTR Path, const VARIANT& Type);
};


	/////////////////////////////////////////////////////////////////////////////
	// _ContactItem wrapper class

	class _ContactItem : public _Item
	{
	public:
		_ContactItem() {}		// Calls COleDispatchDriver default constructor
		_ContactItem(LPDISPATCH pDispatch) : _Item(pDispatch) {}
		_ContactItem(const _ContactItem& dispatchSrc) : _Item(dispatchSrc) {}
		_ContactItem(const _Item& dispatchSrc) : _Item(dispatchSrc) {}

		// Attributes
	public:

		// Operations
	public:
		CString GetAccount();
		void SetAccount(LPCTSTR lpszNewValue);
		DATE GetAnniversary();
		void SetAnniversary(DATE newValue);
		CString GetAssistantName();
		void SetAssistantName(LPCTSTR lpszNewValue);
		CString GetAssistantTelephoneNumber();
		void SetAssistantTelephoneNumber(LPCTSTR lpszNewValue);
		DATE GetBirthday();
		void SetBirthday(DATE newValue);
		CString GetBusiness2TelephoneNumber();
		void SetBusiness2TelephoneNumber(LPCTSTR lpszNewValue);
		CString GetBusinessAddress();
		void SetBusinessAddress(LPCTSTR lpszNewValue);
		CString GetBusinessAddressCity();
		void SetBusinessAddressCity(LPCTSTR lpszNewValue);
		CString GetBusinessAddressCountry();
		void SetBusinessAddressCountry(LPCTSTR lpszNewValue);
		CString GetBusinessAddressPostalCode();
		void SetBusinessAddressPostalCode(LPCTSTR lpszNewValue);
		CString GetBusinessAddressPostOfficeBox();
		void SetBusinessAddressPostOfficeBox(LPCTSTR lpszNewValue);
		CString GetBusinessAddressState();
		void SetBusinessAddressState(LPCTSTR lpszNewValue);
		CString GetBusinessAddressStreet();
		void SetBusinessAddressStreet(LPCTSTR lpszNewValue);
		CString GetBusinessFaxNumber();
		void SetBusinessFaxNumber(LPCTSTR lpszNewValue);
		CString GetBusinessHomePage();
		void SetBusinessHomePage(LPCTSTR lpszNewValue);
		CString GetBusinessTelephoneNumber();
		void SetBusinessTelephoneNumber(LPCTSTR lpszNewValue);
		CString GetCallbackTelephoneNumber();
		void SetCallbackTelephoneNumber(LPCTSTR lpszNewValue);
		CString GetCarTelephoneNumber();
		void SetCarTelephoneNumber(LPCTSTR lpszNewValue);
		CString GetChildren();
		void SetChildren(LPCTSTR lpszNewValue);
		CString GetCompanyAndFullName();
		CString GetCompanyLastFirstNoSpace();
		CString GetCompanyLastFirstSpaceOnly();
		CString GetCompanyMainTelephoneNumber();
		void SetCompanyMainTelephoneNumber(LPCTSTR lpszNewValue);
		CString GetCompanyName();
		void SetCompanyName(LPCTSTR lpszNewValue);
		CString GetComputerNetworkName();
		void SetComputerNetworkName(LPCTSTR lpszNewValue);
		CString GetCustomerID();
		void SetCustomerID(LPCTSTR lpszNewValue);
		CString GetDepartment();
		void SetDepartment(LPCTSTR lpszNewValue);
		CString GetEmail1Address();
		void SetEmail1Address(LPCTSTR lpszNewValue);
		CString GetEmail1AddressType();
		void SetEmail1AddressType(LPCTSTR lpszNewValue);
		CString GetEmail1DisplayName();
		CString GetEmail1EntryID();
		CString GetEmail2Address();
		void SetEmail2Address(LPCTSTR lpszNewValue);
		CString GetEmail2AddressType();
		void SetEmail2AddressType(LPCTSTR lpszNewValue);
		CString GetEmail2DisplayName();
		CString GetEmail2EntryID();
		CString GetEmail3Address();
		void SetEmail3Address(LPCTSTR lpszNewValue);
		CString GetEmail3AddressType();
		void SetEmail3AddressType(LPCTSTR lpszNewValue);
		CString GetEmail3DisplayName();
		CString GetEmail3EntryID();
		CString GetFileAs();
		void SetFileAs(LPCTSTR lpszNewValue);
		CString GetFirstName();
		void SetFirstName(LPCTSTR lpszNewValue);
		CString GetFTPSite();
		void SetFTPSite(LPCTSTR lpszNewValue);
		CString GetFullName();
		void SetFullName(LPCTSTR lpszNewValue);
		CString GetFullNameAndCompany();
		long GetGender();
		void SetGender(long nNewValue);
		CString GetGovernmentIDNumber();
		void SetGovernmentIDNumber(LPCTSTR lpszNewValue);
		CString GetHobby();
		void SetHobby(LPCTSTR lpszNewValue);
		CString GetHome2TelephoneNumber();
		void SetHome2TelephoneNumber(LPCTSTR lpszNewValue);
		CString GetHomeAddress();
		void SetHomeAddress(LPCTSTR lpszNewValue);
		CString GetHomeAddressCity();
		void SetHomeAddressCity(LPCTSTR lpszNewValue);
		CString GetHomeAddressCountry();
		void SetHomeAddressCountry(LPCTSTR lpszNewValue);
		CString GetHomeAddressPostalCode();
		void SetHomeAddressPostalCode(LPCTSTR lpszNewValue);
		CString GetHomeAddressPostOfficeBox();
		void SetHomeAddressPostOfficeBox(LPCTSTR lpszNewValue);
		CString GetHomeAddressState();
		void SetHomeAddressState(LPCTSTR lpszNewValue);
		CString GetHomeAddressStreet();
		void SetHomeAddressStreet(LPCTSTR lpszNewValue);
		CString GetHomeFaxNumber();
		void SetHomeFaxNumber(LPCTSTR lpszNewValue);
		CString GetHomeTelephoneNumber();
		void SetHomeTelephoneNumber(LPCTSTR lpszNewValue);
		CString GetInitials();
		void SetInitials(LPCTSTR lpszNewValue);
		CString GetInternetFreeBusyAddress();
		void SetInternetFreeBusyAddress(LPCTSTR lpszNewValue);
		CString GetISDNNumber();
		void SetISDNNumber(LPCTSTR lpszNewValue);
		CString GetJobTitle();
		void SetJobTitle(LPCTSTR lpszNewValue);
		BOOL GetJournal();
		void SetJournal(BOOL bNewValue);
		CString GetLanguage();
		void SetLanguage(LPCTSTR lpszNewValue);
		CString GetLastFirstAndSuffix();
		CString GetLastFirstNoSpace();
		CString GetLastFirstNoSpaceCompany();
		CString GetLastFirstSpaceOnly();
		CString GetLastFirstSpaceOnlyCompany();
		CString GetLastName();
		void SetLastName(LPCTSTR lpszNewValue);
		CString GetLastNameAndFirstName();
		CString GetMailingAddress();
		void SetMailingAddress(LPCTSTR lpszNewValue);
		CString GetMailingAddressCity();
		void SetMailingAddressCity(LPCTSTR lpszNewValue);
		CString GetMailingAddressCountry();
		void SetMailingAddressCountry(LPCTSTR lpszNewValue);
		CString GetMailingAddressPostalCode();
		void SetMailingAddressPostalCode(LPCTSTR lpszNewValue);
		CString GetMailingAddressPostOfficeBox();
		void SetMailingAddressPostOfficeBox(LPCTSTR lpszNewValue);
		CString GetMailingAddressState();
		void SetMailingAddressState(LPCTSTR lpszNewValue);
		CString GetMailingAddressStreet();
		void SetMailingAddressStreet(LPCTSTR lpszNewValue);
		CString GetManagerName();
		void SetManagerName(LPCTSTR lpszNewValue);
		CString GetMiddleName();
		void SetMiddleName(LPCTSTR lpszNewValue);
		CString GetMobileTelephoneNumber();
		void SetMobileTelephoneNumber(LPCTSTR lpszNewValue);
		CString GetNetMeetingAlias();
		void SetNetMeetingAlias(LPCTSTR lpszNewValue);
		CString GetNetMeetingServer();
		void SetNetMeetingServer(LPCTSTR lpszNewValue);
		CString GetNickName();
		void SetNickName(LPCTSTR lpszNewValue);
		CString GetOfficeLocation();
		void SetOfficeLocation(LPCTSTR lpszNewValue);
		CString GetOrganizationalIDNumber();
		void SetOrganizationalIDNumber(LPCTSTR lpszNewValue);
		CString GetOtherAddress();
		void SetOtherAddress(LPCTSTR lpszNewValue);
		CString GetOtherAddressCity();
		void SetOtherAddressCity(LPCTSTR lpszNewValue);
		CString GetOtherAddressCountry();
		void SetOtherAddressCountry(LPCTSTR lpszNewValue);
		CString GetOtherAddressPostalCode();
		void SetOtherAddressPostalCode(LPCTSTR lpszNewValue);
		CString GetOtherAddressPostOfficeBox();
		void SetOtherAddressPostOfficeBox(LPCTSTR lpszNewValue);
		CString GetOtherAddressState();
		void SetOtherAddressState(LPCTSTR lpszNewValue);
		CString GetOtherAddressStreet();
		void SetOtherAddressStreet(LPCTSTR lpszNewValue);
		CString GetOtherFaxNumber();
		void SetOtherFaxNumber(LPCTSTR lpszNewValue);
		CString GetOtherTelephoneNumber();
		void SetOtherTelephoneNumber(LPCTSTR lpszNewValue);
		CString GetPagerNumber();
		void SetPagerNumber(LPCTSTR lpszNewValue);
		CString GetPersonalHomePage();
		void SetPersonalHomePage(LPCTSTR lpszNewValue);
		CString GetPrimaryTelephoneNumber();
		void SetPrimaryTelephoneNumber(LPCTSTR lpszNewValue);
		CString GetProfession();
		void SetProfession(LPCTSTR lpszNewValue);
		CString GetRadioTelephoneNumber();
		void SetRadioTelephoneNumber(LPCTSTR lpszNewValue);
		CString GetReferredBy();
		void SetReferredBy(LPCTSTR lpszNewValue);
		long GetSelectedMailingAddress();
		void SetSelectedMailingAddress(long nNewValue);
		CString GetSpouse();
		void SetSpouse(LPCTSTR lpszNewValue);
		CString GetSuffix();
		void SetSuffix(LPCTSTR lpszNewValue);
		CString GetTelexNumber();
		void SetTelexNumber(LPCTSTR lpszNewValue);
		CString GetTitle();
		void SetTitle(LPCTSTR lpszNewValue);
		CString GetTTYTDDTelephoneNumber();
		void SetTTYTDDTelephoneNumber(LPCTSTR lpszNewValue);
		CString GetUser1();
		void SetUser1(LPCTSTR lpszNewValue);
		CString GetUser2();
		void SetUser2(LPCTSTR lpszNewValue);
		CString GetUser3();
		void SetUser3(LPCTSTR lpszNewValue);
		CString GetUser4();
		void SetUser4(LPCTSTR lpszNewValue);
		CString GetWebPage();
		void SetWebPage(LPCTSTR lpszNewValue);
		CString GetYomiCompanyName();
		void SetYomiCompanyName(LPCTSTR lpszNewValue);
		CString GetYomiFirstName();
		void SetYomiFirstName(LPCTSTR lpszNewValue);
		CString GetYomiLastName();
		void SetYomiLastName(LPCTSTR lpszNewValue);
		LPDISPATCH ForwardAsVcard();
		LPDISPATCH GetLinks();
		LPDISPATCH GetItemProperties();
		CString GetLastFirstNoSpaceAndSuffix();
		long GetDownloadState();
		void ShowCategoriesDialog();
		CString GetIMAddress();
		void SetIMAddress(LPCTSTR lpszNewValue);
		long GetMarkForDownload();
		void SetMarkForDownload(long nNewValue);
		void SetEmail1DisplayName(LPCTSTR lpszNewValue);
		void SetEmail2DisplayName(LPCTSTR lpszNewValue);
		void SetEmail3DisplayName(LPCTSTR lpszNewValue);
		BOOL GetIsConflict();
		BOOL GetAutoResolvedWinner();
		LPDISPATCH GetConflicts();
		void AddPicture(LPCTSTR Path);
		void RemovePicture();
		BOOL GetHasPicture();
		LPDISPATCH GetPropertyAccessor();
		LPDISPATCH ForwardAsBusinessCard();
		void ShowBusinessCardEditor();
		void SaveBusinessCardImage(LPCTSTR Path);
		void ShowCheckPhoneDialog(long PhoneNumber);
		CString GetTaskSubject();
		void SetTaskSubject(LPCTSTR lpszNewValue);
		DATE GetTaskDueDate();
		void SetTaskDueDate(DATE newValue);
		DATE GetTaskStartDate();
		void SetTaskStartDate(DATE newValue);
		DATE GetTaskCompletedDate();
		void SetTaskCompletedDate(DATE newValue);
		DATE GetToDoTaskOrdinal();
		void SetToDoTaskOrdinal(DATE newValue);
		BOOL GetReminderOverrideDefault();
		void SetReminderOverrideDefault(BOOL bNewValue);
		BOOL GetReminderPlaySound();
		void SetReminderPlaySound(BOOL bNewValue);
		BOOL GetReminderSet();
		void SetReminderSet(BOOL bNewValue);
		CString GetReminderSoundFile();
		void SetReminderSoundFile(LPCTSTR lpszNewValue);
		DATE GetReminderTime();
		void SetReminderTime(DATE newValue);
		void MarkAsTask(long MarkInterval);
		void ClearTaskFlag();
		BOOL GetIsMarkedAsTask();
		CString GetBusinessCardLayoutXml();
		void SetBusinessCardLayoutXml(LPCTSTR lpszNewValue);
		void ResetBusinessCard();
		void AddBusinessCardLogoPicture(LPCTSTR Path);
		long GetBusinessCardType();
	};

	/////////////////////////////////////////////////////////////////////////////
	// _TaskItem wrapper class

	class _TaskItem : public _Item
	{
	public:
		_TaskItem() {}		// Calls _Item default constructor
		_TaskItem(LPDISPATCH pDispatch) : _Item(pDispatch) {}
		_TaskItem(const _TaskItem& dispatchSrc) : _Item(dispatchSrc) {}
		_TaskItem(const _Item& dispatchSrc) : _Item(dispatchSrc) {}

		// Attributes
	public:

		// Operations
	public:
		long GetActualWork();
		void SetActualWork(long nNewValue);
		CString GetCardData();
		void SetCardData(LPCTSTR lpszNewValue);
		BOOL GetComplete();
		void SetComplete(BOOL bNewValue);
		CString GetContactNames();
		void SetContactNames(LPCTSTR lpszNewValue);
		DATE GetDateCompleted();
		void SetDateCompleted(DATE newValue);
		long GetDelegationState();
		CString GetDelegator();
		DATE GetDueDate();
		void SetDueDate(DATE newValue);
		BOOL GetIsRecurring();
		long GetOrdinal();
		void SetOrdinal(long nNewValue);
		CString GetOwner();
		void SetOwner(LPCTSTR lpszNewValue);
		long GetOwnership();
		long GetPercentComplete();
		void SetPercentComplete(long nNewValue);
		LPDISPATCH GetRecipients();
		DATE GetReminderTime();
		void SetReminderTime(DATE newValue);
		BOOL GetReminderOverrideDefault();
		void SetReminderOverrideDefault(BOOL bNewValue);
		BOOL GetReminderPlaySound();
		void SetReminderPlaySound(BOOL bNewValue);
		BOOL GetReminderSet();
		void SetReminderSet(BOOL bNewValue);
		CString GetReminderSoundFile();
		void SetReminderSoundFile(LPCTSTR lpszNewValue);
		long GetResponseState();
		CString GetRole();
		void SetRole(LPCTSTR lpszNewValue);
		CString GetSchedulePlusPriority();
		void SetSchedulePlusPriority(LPCTSTR lpszNewValue);
		DATE GetStartDate();
		void SetStartDate(DATE newValue);
		long GetStatus();
		void SetStatus(long nNewValue);
		CString GetStatusOnCompletionRecipients();
		void SetStatusOnCompletionRecipients(LPCTSTR lpszNewValue);
		CString GetStatusUpdateRecipients();
		void SetStatusUpdateRecipients(LPCTSTR lpszNewValue);
		BOOL GetTeamTask();
		void SetTeamTask(BOOL bNewValue);
		long GetTotalWork();
		void SetTotalWork(long nNewValue);
		LPDISPATCH Assign();
		void CancelResponseState();
		void ClearRecurrencePattern();
		LPDISPATCH GetRecurrencePattern();
		void MarkComplete();
		LPDISPATCH Respond(long Response, const VARIANT& fNoUI, const VARIANT& fAdditionalTextDialog);
		void Send();
		BOOL SkipRecurrence();
		LPDISPATCH StatusReport();
		LPDISPATCH GetLinks();
		long GetDownloadState();
		void ShowCategoriesDialog();
		LPDISPATCH GetItemProperties();
		long GetInternetCodepage();
		void SetInternetCodepage(long nNewValue);
		long GetMarkForDownload();
		void SetMarkForDownload(long nNewValue);
		BOOL GetIsConflict();
		BOOL GetAutoResolvedWinner();
		LPDISPATCH GetConflicts();
		LPDISPATCH GetPropertyAccessor();
		LPDISPATCH GetSendUsingAccount();
		void SetSendUsingAccount(LPDISPATCH newValue);
		DATE GetToDoTaskOrdinal();
		void SetToDoTaskOrdinal(DATE newValue);
	};

	/////////////////////////////////////////////////////////////////////////////
	// _AppointmentItem wrapper class

	class _AppointmentItem : public _Item
	{
	public:
		_AppointmentItem() {}		// Calls _Item default constructor
		_AppointmentItem(LPDISPATCH pDispatch) : _Item(pDispatch) {}
		_AppointmentItem(const _AppointmentItem& dispatchSrc) : _Item(dispatchSrc) {}
		_AppointmentItem(const _Item& dispatchSrc) : _Item(dispatchSrc) {}

		// Attributes
	public:

		// Operations
	public:
		BOOL GetAllDayEvent();
		void SetAllDayEvent(BOOL bNewValue);
		long GetBusyStatus();
		void SetBusyStatus(long nNewValue);
		long GetDuration();
		void SetDuration(long nNewValue);
		DATE GetEnd();
		void SetEnd(DATE newValue);
		BOOL GetIsRecurring();
		CString GetLocation();
		void SetLocation(LPCTSTR lpszNewValue);
		long GetMeetingStatus();
		void SetMeetingStatus(long nNewValue);
		CString GetOptionalAttendees();
		void SetOptionalAttendees(LPCTSTR lpszNewValue);
		CString GetOrganizer();
		LPDISPATCH GetRecipients();
		long GetRecurrenceState();
		long GetReminderMinutesBeforeStart();
		void SetReminderMinutesBeforeStart(long nNewValue);
		BOOL GetReminderOverrideDefault();
		void SetReminderOverrideDefault(BOOL bNewValue);
		BOOL GetReminderPlaySound();
		void SetReminderPlaySound(BOOL bNewValue);
		BOOL GetReminderSet();
		void SetReminderSet(BOOL bNewValue);
		CString GetReminderSoundFile();
		void SetReminderSoundFile(LPCTSTR lpszNewValue);
		DATE GetReplyTime();
		void SetReplyTime(DATE newValue);
		CString GetRequiredAttendees();
		void SetRequiredAttendees(LPCTSTR lpszNewValue);
		CString GetResources();
		void SetResources(LPCTSTR lpszNewValue);
		BOOL GetResponseRequested();
		void SetResponseRequested(BOOL bNewValue);
		long GetResponseStatus();
		DATE GetStart();
		void SetStart(DATE newValue);
		void ClearRecurrencePattern();
		LPDISPATCH ForwardAsVcal();
		LPDISPATCH GetRecurrencePattern();
		LPDISPATCH Respond(long Response, const VARIANT& fNoUI, const VARIANT& fAdditionalTextDialog);
		void Send();
		LPDISPATCH GetLinks();
		LPDISPATCH GetItemProperties();
		long GetDownloadState();
		void ShowCategoriesDialog();
		long GetInternetCodepage();
		void SetInternetCodepage(long nNewValue);
		long GetMarkForDownload();
		void SetMarkForDownload(long nNewValue);
		BOOL GetIsConflict();
		CString GetMeetingWorkspaceURL();
		BOOL GetAutoResolvedWinner();
		LPDISPATCH GetConflicts();
		LPDISPATCH GetPropertyAccessor();
		LPDISPATCH GetSendUsingAccount();
		void SetSendUsingAccount(LPDISPATCH newValue);
		CString GetGlobalAppointmentID();
		BOOL GetForceUpdateToAllAttendees();
		void SetForceUpdateToAllAttendees(BOOL bNewValue);
		DATE GetStartUTC();
		void SetStartUTC(DATE newValue);
		DATE GetEndUTC();
		void SetEndUTC(DATE newValue);
		DATE GetStartInStartTimeZone();
		void SetStartInStartTimeZone(DATE newValue);
		DATE GetEndInEndTimeZone();
		void SetEndInEndTimeZone(DATE newValue);
		LPDISPATCH GetStartTimeZone();
		void SetStartTimeZone(LPDISPATCH newValue);
		LPDISPATCH GetEndTimeZone();
		void SetEndTimeZone(LPDISPATCH newValue);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _JournalItem wrapper class

	class _JournalItem : public _Item
	{
	public:
		_JournalItem() {}		// Calls _Item default constructor
		_JournalItem(LPDISPATCH pDispatch) : _Item(pDispatch) {}
		_JournalItem(const _JournalItem& dispatchSrc) : _Item(dispatchSrc) {}
		_JournalItem(const _Item& dispatchSrc) : _Item(dispatchSrc) {}

		// Attributes
	public:

		// Operations
	public:
		CString GetContactNames();
		void SetContactNames(LPCTSTR lpszNewValue);
		BOOL GetDocPosted();
		void SetDocPosted(BOOL bNewValue);
		BOOL GetDocPrinted();
		void SetDocPrinted(BOOL bNewValue);
		BOOL GetDocRouted();
		void SetDocRouted(BOOL bNewValue);
		BOOL GetDocSaved();
		void SetDocSaved(BOOL bNewValue);
		long GetDuration();
		void SetDuration(long nNewValue);
		DATE GetEnd();
		void SetEnd(DATE newValue);
		CString GetType();
		void SetType(LPCTSTR lpszNewValue);
		LPDISPATCH GetRecipients();
		DATE GetStart();
		void SetStart(DATE newValue);
		LPDISPATCH Forward();
		LPDISPATCH Reply();
		LPDISPATCH ReplyAll();
		void StartTimer();
		void StopTimer();
		LPDISPATCH GetLinks();
		long GetDownloadState();
		void ShowCategoriesDialog();
		LPDISPATCH GetItemProperties();
		long GetMarkForDownload();
		void SetMarkForDownload(long nNewValue);
		BOOL GetIsConflict();
		BOOL GetAutoResolvedWinner();
		LPDISPATCH GetConflicts();
		LPDISPATCH GetPropertyAccessor();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _NoteItem wrapper class

	class _NoteItem : public _Item
	{
	public:
		_NoteItem() {}		// Calls _Item default constructor
		_NoteItem(LPDISPATCH pDispatch) : _Item(pDispatch) {}
		_NoteItem(const _NoteItem& dispatchSrc) : _Item(dispatchSrc) {}
		_NoteItem(const _Item& dispatchSrc) : _Item(dispatchSrc) {}

		// Attributes
	public:

		// Operations
	public:
		long GetHeight();
		void SetHeight(long nNewValue);
		long GetLeft();
		void SetLeft(long nNewValue);
		long GetTop();
		void SetTop(long nNewValue);
		long GetWidth();
		void SetWidth(long nNewValue);
		LPDISPATCH GetLinks();
		long GetDownloadState();
		LPDISPATCH GetItemProperties();
		long GetMarkForDownload();
		void SetMarkForDownload(long nNewValue);
		BOOL GetIsConflict();
		BOOL GetAutoResolvedWinner();
		LPDISPATCH GetConflicts();
		LPDISPATCH GetPropertyAccessor();
	};

	/////////////////////////////////////////////////////////////////////////////
	// _MailItem wrapper class

	class _MailItem : public _Item
	{
	public:
		_MailItem() {}		// Calls _Item default constructor
		_MailItem(LPDISPATCH pDispatch) : _Item(pDispatch) {}
		_MailItem(const _MailItem& dispatchSrc) : _Item(dispatchSrc) {}
		_MailItem(const _Item& dispatchSrc) : _Item(dispatchSrc) {}

		// Attributes
	public:

		// Operations
	public:
		BOOL GetAlternateRecipientAllowed();
		void SetAlternateRecipientAllowed(BOOL bNewValue);
		BOOL GetAutoForwarded();
		void SetAutoForwarded(BOOL bNewValue);
		CString GetBcc();
		void SetBcc(LPCTSTR lpszNewValue);
		CString GetCc();
		void SetCc(LPCTSTR lpszNewValue);
		DATE GetDeferredDeliveryTime();
		void SetDeferredDeliveryTime(DATE newValue);
		BOOL GetDeleteAfterSubmit();
		void SetDeleteAfterSubmit(BOOL bNewValue);
		DATE GetExpiryTime();
		void SetExpiryTime(DATE newValue);
		CString GetFlagRequest();
		void SetFlagRequest(LPCTSTR lpszNewValue);
		CString GetHTMLBody();
		void SetHTMLBody(LPCTSTR lpszNewValue);
		BOOL GetOriginatorDeliveryReportRequested();
		void SetOriginatorDeliveryReportRequested(BOOL bNewValue);
		BOOL GetReadReceiptRequested();
		void SetReadReceiptRequested(BOOL bNewValue);
		CString GetReceivedByEntryID();
		CString GetReceivedByName();
		CString GetReceivedOnBehalfOfEntryID();
		CString GetReceivedOnBehalfOfName();
		DATE GetReceivedTime();
		BOOL GetRecipientReassignmentProhibited();
		void SetRecipientReassignmentProhibited(BOOL bNewValue);
		LPDISPATCH GetRecipients();
		BOOL GetReminderOverrideDefault();
		void SetReminderOverrideDefault(BOOL bNewValue);
		BOOL GetReminderPlaySound();
		void SetReminderPlaySound(BOOL bNewValue);
		BOOL GetReminderSet();
		void SetReminderSet(BOOL bNewValue);
		CString GetReminderSoundFile();
		void SetReminderSoundFile(LPCTSTR lpszNewValue);
		DATE GetReminderTime();
		void SetReminderTime(DATE newValue);
		long GetRemoteStatus();
		void SetRemoteStatus(long nNewValue);
		CString GetReplyRecipientNames();
		LPDISPATCH GetReplyRecipients();
		LPDISPATCH GetSaveSentMessageFolder();
		void SetRefSaveSentMessageFolder(LPDISPATCH newValue);
		CString GetSenderName();
		BOOL GetSent();
		DATE GetSentOn();
		CString GetSentOnBehalfOfName();
		void SetSentOnBehalfOfName(LPCTSTR lpszNewValue);
		BOOL GetSubmitted();
		CString GetTo();
		void SetTo(LPCTSTR lpszNewValue);
		CString GetVotingOptions();
		void SetVotingOptions(LPCTSTR lpszNewValue);
		CString GetVotingResponse();
		void SetVotingResponse(LPCTSTR lpszNewValue);
		void ClearConversationIndex();
		LPDISPATCH Forward();
		LPDISPATCH Reply();
		LPDISPATCH ReplyAll();
		void Send();
		LPDISPATCH GetLinks();
		LPDISPATCH GetItemProperties();
		long GetBodyFormat();
		void SetBodyFormat(long nNewValue);
		long GetDownloadState();
		void ShowCategoriesDialog();
		long GetInternetCodepage();
		void SetInternetCodepage(long nNewValue);
		long GetMarkForDownload();
		void SetMarkForDownload(long nNewValue);
		BOOL GetIsConflict();
		BOOL GetAutoResolvedWinner();
		LPDISPATCH GetConflicts();
		CString GetSenderEmailAddress();
		CString GetSenderEmailType();
		long GetPermission();
		void SetPermission(long nNewValue);
		long GetPermissionService();
		void SetPermissionService(long nNewValue);
		LPDISPATCH GetPropertyAccessor();
		void AddBusinessCard(LPDISPATCH contact);
		LPDISPATCH GetSendUsingAccount();
		void SetSendUsingAccount(LPDISPATCH newValue);
		CString GetTaskSubject();
		void SetTaskSubject(LPCTSTR lpszNewValue);
		DATE GetTaskDueDate();
		void SetTaskDueDate(DATE newValue);
		DATE GetTaskStartDate();
		void SetTaskStartDate(DATE newValue);
		DATE GetTaskCompletedDate();
		void SetTaskCompletedDate(DATE newValue);
		DATE GetToDoTaskOrdinal();
		void SetToDoTaskOrdinal(DATE newValue);
		void MarkAsTask(long MarkInterval);
		void ClearTaskFlag();
		BOOL GetIsMarkedAsTask();
	};
	/////////////////////////////////////////////////////////////////////////////
	// Attachments wrapper class

	class Attachments : public COleDispatchDriver
	{
	public:
		Attachments() {}		// Calls COleDispatchDriver default constructor
		Attachments(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
		Attachments(const Attachments& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

		// Attributes
	public:

		// Operations
	public:
		LPDISPATCH GetApplication();
		long GetClass();
		LPDISPATCH GetSession();
		LPDISPATCH GetParent();
		long GetCount();
		LPDISPATCH Item(const VARIANT& Index);
		LPDISPATCH Add(const VARIANT& Source, const VARIANT& Type, const VARIANT& Position, const VARIANT& DisplayName);
		void Remove(long Index);
	};
	/////////////////////////////////////////////////////////////////////////////
	// Attachment wrapper class

	class Attachment : public COleDispatchDriver
	{
	public:
		Attachment() {}		// Calls COleDispatchDriver default constructor
		Attachment(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
		Attachment(const Attachment& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

		// Attributes
	public:

		// Operations
	public:
		LPDISPATCH GetApplication();
		long GetClass();
		LPDISPATCH GetSession();
		LPDISPATCH GetParent();
		CString GetDisplayName();
		void SetDisplayName(LPCTSTR lpszNewValue);
		CString GetFileName();
		long GetIndex();
		CString GetPathName();
		long GetPosition();
		void SetPosition(long nNewValue);
		long GetType();
		void Delete();
		void SaveAsFile(LPCTSTR Path);
		LPDISPATCH GetPropertyAccessor();
		long GetSize();
		long GetBlockLevel();
	};

	/////////////////////////////////////////////////////////////////////////////
	// _Application wrapper class

	class _Application : public COleDispatchDriver
	{
	public:
		_Application() {}		// Calls COleDispatchDriver default constructor
		_Application(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
		_Application(const _Application& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

		// Attributes
	public:

		// Operations
	public:
		LPDISPATCH GetApplication();
		long GetClass();
		LPDISPATCH GetSession();
		LPDISPATCH GetParent();
		CString GetName();
		CString GetVersion();
		LPDISPATCH ActiveExplorer();
		LPDISPATCH ActiveInspector();
		LPDISPATCH CreateItem(long ItemType);
		LPDISPATCH CreateItemFromTemplate(LPCTSTR TemplatePath, const VARIANT& InFolder);
		LPDISPATCH CreateObject(LPCTSTR ObjectName);
		LPDISPATCH GetNamespace(LPCTSTR Type);
		void Quit();
		LPDISPATCH GetCOMAddIns();
		LPDISPATCH GetExplorers();
		LPDISPATCH GetInspectors();
		LPDISPATCH GetLanguageSettings();
		CString GetProductCode();
		LPDISPATCH ActiveWindow();
		LPDISPATCH CopyFile(LPCTSTR FilePath, LPCTSTR DestFolderPath);
		LPDISPATCH AdvancedSearch(LPCTSTR Scope, const VARIANT& Filter, const VARIANT& SearchSubFolders, const VARIANT& Tag);
		BOOL IsSearchSynchronous(LPCTSTR LookInFolders);
		LPDISPATCH GetReminders();
		CString GetDefaultProfileName();
		BOOL GetIsTrusted();
		LPDISPATCH GetObjectReference(LPDISPATCH Item, long ReferenceType);
		LPDISPATCH GetAssistance();
		LPDISPATCH GetTimeZones();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _NameSpace wrapper class

	class _NameSpace : public COleDispatchDriver
	{
	public:
		_NameSpace() {}		// Calls COleDispatchDriver default constructor
		_NameSpace(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
		_NameSpace(const _NameSpace& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

		// Attributes
	public:

		// Operations
	public:
		LPDISPATCH GetApplication();
		long GetClass();
		LPDISPATCH GetSession();
		LPDISPATCH GetParent();
		LPDISPATCH GetCurrentUser();
		LPDISPATCH GetFolders();
		CString GetType();
		LPDISPATCH GetAddressLists();
		LPDISPATCH CreateRecipient(LPCTSTR RecipientName);
		LPDISPATCH GetDefaultFolder(long FolderType);
		LPDISPATCH GetFolderFromID(LPCTSTR EntryIDFolder, const VARIANT& EntryIDStore);
		LPDISPATCH GetItemFromID(LPCTSTR EntryIDItem, const VARIANT& EntryIDStore);
		LPDISPATCH GetRecipientFromID(LPCTSTR EntryID);
		LPDISPATCH GetSharedDefaultFolder(LPDISPATCH Recipient, long FolderType);
		void Logoff();
		void Logon(const VARIANT& Profile, const VARIANT& Password, const VARIANT& ShowDialog, const VARIANT& NewSession);
		LPDISPATCH PickFolder();
		LPDISPATCH GetSyncObjects();
		void AddStore(const VARIANT& Store);
		void RemoveStore(LPDISPATCH Folder);
		BOOL GetOffline();
		void Dial(const VARIANT& ContactItem);
		long GetExchangeConnectionMode();
		void AddStoreEx(const VARIANT& Store, long Type);
		LPDISPATCH GetAccounts();
		CString GetCurrentProfileName();
		LPDISPATCH GetStores();
		LPDISPATCH GetSelectNamesDialog();
		void SendAndReceive(BOOL showProgressDialog);
		LPDISPATCH GetDefaultStore();
		LPDISPATCH GetAddressEntryFromID(LPCTSTR ID);
		LPDISPATCH GetGlobalAddressList();
		LPDISPATCH GetStoreFromID(LPCTSTR ID);
		LPDISPATCH GetCategories();
		LPDISPATCH OpenSharedFolder(LPCTSTR Path, const VARIANT& Name, const VARIANT& DownloadAttachments, const VARIANT& UseTTL);
		LPDISPATCH OpenSharedItem(LPCTSTR Path);
		LPDISPATCH CreateSharingItem(const VARIANT& Context, const VARIANT& Provider);
		CString GetExchangeMailboxServerName();
		CString GetExchangeMailboxServerVersion();
		BOOL CompareEntryIDs(LPCTSTR FirstEntryID, LPCTSTR SecondEntryID);
		CString GetAutoDiscoverXml();
		long GetAutoDiscoverConnectionMode();
	};


	/////////////////////////////////////////////////////////////////////////////
	// MAPIFolder wrapper class

	class MAPIFolder : public COleDispatchDriver
	{
	public:
		MAPIFolder() {}		// Calls COleDispatchDriver default constructor
		MAPIFolder(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
		MAPIFolder(const MAPIFolder& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

		// Attributes
	public:

		// Operations
	public:
		LPDISPATCH GetApplication();
		long GetClass();
		LPDISPATCH GetSession();
		LPDISPATCH GetParent();
		long GetDefaultItemType();
		CString GetDefaultMessageClass();
		CString GetDescription();
		void SetDescription(LPCTSTR lpszNewValue);
		CString GetEntryID();
		LPDISPATCH GetFolders();
		LPDISPATCH GetItems();
		CString GetName();
		void SetName(LPCTSTR lpszNewValue);
		CString GetStoreID();
		long GetUnReadItemCount();
		LPDISPATCH CopyTo(LPDISPATCH DestinationFolder);
		void Delete();
		void Display();
		LPDISPATCH GetExplorer(const VARIANT& DisplayMode);
		void MoveTo(LPDISPATCH DestinationFolder);
		BOOL GetWebViewOn();
		void SetWebViewOn(BOOL bNewValue);
		CString GetWebViewURL();
		void SetWebViewURL(LPCTSTR lpszNewValue);
		void AddToPFFavorites();
		CString GetAddressBookName();
		void SetAddressBookName(LPCTSTR lpszNewValue);
		BOOL GetShowAsOutlookAB();
		void SetShowAsOutlookAB(BOOL bNewValue);
		CString GetFolderPath();
		BOOL GetInAppFolderSyncObject();
		void SetInAppFolderSyncObject(BOOL bNewValue);
		LPDISPATCH GetCurrentView();
		BOOL GetCustomViewsOnly();
		void SetCustomViewsOnly(BOOL bNewValue);
		LPDISPATCH GetViews();
		BOOL GetIsSharePointFolder();
		long GetShowItemCount();
		void SetShowItemCount(long nNewValue);
		LPDISPATCH GetStore();
		LPDISPATCH GetStorage(LPCTSTR StorageIdentifier, long StorageIdentifierType);
		LPDISPATCH GetTable(const VARIANT& Filter, const VARIANT& TableContents);
		LPDISPATCH GetPropertyAccessor();
		LPDISPATCH GetCalendarExporter();
		LPDISPATCH GetUserDefinedProperties();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _Items wrapper class

	class _Items : public COleDispatchDriver
	{
	public:
		_Items() {}		// Calls COleDispatchDriver default constructor
		_Items(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
		_Items(const _Items& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

		// Attributes
	public:

		// Operations
	public:
		LPDISPATCH GetApplication();
		long GetClass();
		LPDISPATCH GetSession();
		LPDISPATCH GetParent();
		long GetCount();
		LPDISPATCH Item(const VARIANT& Index);
		BOOL GetIncludeRecurrences();
		void SetIncludeRecurrences(BOOL bNewValue);
		LPDISPATCH Add(const VARIANT& Type);
		LPDISPATCH Find(LPCTSTR Filter);
		LPDISPATCH FindNext();
		LPDISPATCH GetFirst();
		LPDISPATCH GetLast();
		LPDISPATCH GetNext();
		LPDISPATCH GetPrevious();
		void Remove(long Index);
		void ResetColumns();
		LPDISPATCH Restrict(LPCTSTR Filter);
		void SetColumns(LPCTSTR Columns);
		void Sort(LPCTSTR Property_, const VARIANT& Descending);
	};

	/////////////////////////////////////////////////////////////////////////////
	// _Explorer wrapper class

	class _Explorer : public COleDispatchDriver
	{
	public:
		_Explorer() {}		// Calls COleDispatchDriver default constructor
		_Explorer(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
		_Explorer(const _Explorer& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

		// Attributes
	public:

		// Operations
	public:
		LPDISPATCH GetApplication();
		long GetClass();
		LPDISPATCH GetSession();
		LPDISPATCH GetParent();
		LPDISPATCH GetCommandBars();
		LPDISPATCH GetCurrentFolder();
		void SetRefCurrentFolder(LPDISPATCH newValue);
		void Close();
		void Display();
		CString GetCaption();
		VARIANT GetCurrentView();
		void SetCurrentView(const VARIANT& newValue);
		long GetHeight();
		void SetHeight(long nNewValue);
		long GetLeft();
		void SetLeft(long nNewValue);
		LPDISPATCH GetPanes();
		LPDISPATCH GetSelection();
		long GetTop();
		void SetTop(long nNewValue);
		long GetWidth();
		void SetWidth(long nNewValue);
		long GetWindowState();
		void SetWindowState(long nNewValue);
		void Activate();
		BOOL IsPaneVisible(long Pane);
		void ShowPane(long Pane, BOOL Visible);
		LPDISPATCH GetHTMLDocument();
		LPDISPATCH GetNavigationPane();
		void ClearSearch();
		void Search(LPCTSTR Query, long SearchScope);
	};

	/////////////////////////////////////////////////////////////////////////////
	// Selection wrapper class

	class Selection : public COleDispatchDriver
	{
	public:
		Selection() {}		// Calls COleDispatchDriver default constructor
		Selection(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
		Selection(const Selection& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

		// Attributes
	public:

		// Operations
	public:
		LPDISPATCH GetApplication();
		long GetClass();
		LPDISPATCH GetSession();
		LPDISPATCH GetParent();
		long GetCount();
		LPDISPATCH Item(const VARIANT& Index);
	};

	/////////////////////////////////////////////////////////////////////////////
	// Recipient wrapper class
	/*
	class Recipient : public COleDispatchDriver
	{
	public:
	Recipient() {}		// Calls COleDispatchDriver default constructor
	Recipient(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Recipient(const Recipient& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	CString GetAddress();
	LPDISPATCH GetAddressEntry();
	void SetRefAddressEntry(LPDISPATCH newValue);
	CString GetAutoResponse();
	void SetAutoResponse(LPCTSTR lpszNewValue);
	long GetDisplayType();
	CString GetEntryID();
	long GetIndex();
	long GetMeetingResponseStatus();
	CString GetName();
	BOOL GetResolved();
	long GetTrackingStatus();
	void SetTrackingStatus(long nNewValue);
	DATE GetTrackingStatusTime();
	void SetTrackingStatusTime(DATE newValue);
	long GetType();
	void SetType(long nNewValue);
	void Delete();
	CString FreeBusy(DATE Start, long MinPerChar, const VARIANT& CompleteFormat);
	BOOL Resolve();
	LPDISPATCH GetPropertyAccessor();
	};
	/////////////////////////////////////////////////////////////////////////////
	// AddressEntry wrapper class

	class AddressEntry : public COleDispatchDriver
	{
	public:
	AddressEntry() {}		// Calls COleDispatchDriver default constructor
	AddressEntry(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	AddressEntry(const AddressEntry& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	CString GetAddress();
	void SetAddress(LPCTSTR lpszNewValue);
	long GetDisplayType();
	CString GetId();
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	CString GetType();
	void SetType(LPCTSTR lpszNewValue);
	void Delete();
	void Details(const VARIANT& HWnd);
	CString GetFreeBusy(DATE Start, long MinPerChar, const VARIANT& CompleteFormat);
	void Update(const VARIANT& MakePermanent, const VARIANT& Refresh);
	LPDISPATCH GetContact();
	LPDISPATCH GetExchangeUser();
	long GetAddressEntryUserType();
	LPDISPATCH GetExchangeDistributionList();
	LPDISPATCH GetPropertyAccessor();
	};
	/////////////////////////////////////////////////////////////////////////////
	// AddressEntries wrapper class

	class AddressEntries : public COleDispatchDriver
	{
	public:
	AddressEntries() {}		// Calls COleDispatchDriver default constructor
	AddressEntries(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	AddressEntries(const AddressEntries& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH Add(LPCTSTR Type, const VARIANT& Name, const VARIANT& Address);
	LPDISPATCH GetFirst();
	LPDISPATCH GetLast();
	LPDISPATCH GetNext();
	LPDISPATCH GetPrevious();
	void Sort(const VARIANT& Property_, const VARIANT& Order);
	};
	*/
	/////////////////////////////////////////////////////////////////////////////
	// _Folders wrapper class
	/*
	class _Folders : public COleDispatchDriver
	{
	public:
	_Folders() {}		// Calls COleDispatchDriver default constructor
	_Folders(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Folders(const _Folders& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH Add(LPCTSTR Name, const VARIANT& Type);
	LPDISPATCH GetFirst();
	LPDISPATCH GetLast();
	LPDISPATCH GetNext();
	LPDISPATCH GetPrevious();
	void Remove(long Index);
	};
	*/
	/////////////////////////////////////////////////////////////////////////////
	// Panes wrapper class
	/*
	class Panes : public COleDispatchDriver
	{
	public:
	Panes() {}		// Calls COleDispatchDriver default constructor
	Panes(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Panes(const Panes& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	};
	*/
	/////////////////////////////////////////////////////////////////////////////
	// _NavigationPane wrapper class
	/*
	class _NavigationPane : public COleDispatchDriver
	{
	public:
	_NavigationPane() {}		// Calls COleDispatchDriver default constructor
	_NavigationPane(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_NavigationPane(const _NavigationPane& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	BOOL GetIsCollapsed();
	void SetIsCollapsed(BOOL bNewValue);
	LPDISPATCH GetCurrentModule();
	void SetRefCurrentModule(LPDISPATCH newValue);
	long GetDisplayedModuleCount();
	void SetDisplayedModuleCount(long nNewValue);
	LPDISPATCH GetModules();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _NavigationModule wrapper class

	class _NavigationModule : public COleDispatchDriver
	{
	public:
	_NavigationModule() {}		// Calls COleDispatchDriver default constructor
	_NavigationModule(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_NavigationModule(const _NavigationModule& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetNavigationModuleType();
	CString GetName();
	long GetPosition();
	void SetPosition(long nNewValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _NavigationModules wrapper class

	class _NavigationModules : public COleDispatchDriver
	{
	public:
	_NavigationModules() {}		// Calls COleDispatchDriver default constructor
	_NavigationModules(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_NavigationModules(const _NavigationModules& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH GetNavigationModule(long ModuleType);
	};
	/////////////////////////////////////////////////////////////////////////////
	// View wrapper class

	class View : public COleDispatchDriver
	{
	public:
	View() {}		// Calls COleDispatchDriver default constructor
	View(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	View(const View& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	void Apply();
	LPDISPATCH Copy(LPCTSTR Name, long SaveOption);
	void Delete();
	void Reset();
	void Save();
	CString GetLanguage();
	void SetLanguage(LPCTSTR lpszNewValue);
	BOOL GetLockUserChanges();
	void SetLockUserChanges(BOOL bNewValue);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	long GetSaveOption();
	BOOL GetStandard();
	long GetViewType();
	CString GetXml();
	void SetXml(LPCTSTR lpszNewValue);
	void GoToDate(DATE Date);
	CString GetFilter();
	void SetFilter(LPCTSTR lpszNewValue);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _Views wrapper class

	class _Views : public COleDispatchDriver
	{
	public:
	_Views() {}		// Calls COleDispatchDriver default constructor
	_Views(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Views(const _Views& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH Add(LPCTSTR Name, long ViewType, long SaveOption);
	void Remove(const VARIANT& Index);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _Store wrapper class

	class _Store : public COleDispatchDriver
	{
	public:
	_Store() {}		// Calls COleDispatchDriver default constructor
	_Store(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Store(const _Store& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	CString GetDisplayName();
	CString GetStoreID();
	long GetExchangeStoreType();
	CString GetFilePath();
	BOOL GetIsCachedExchange();
	BOOL GetIsDataFileStore();
	LPDISPATCH GetRootFolder();
	LPDISPATCH GetSearchFolders();
	BOOL GetIsOpen();
	LPDISPATCH GetRules();
	LPDISPATCH GetPropertyAccessor();
	LPDISPATCH GetSpecialFolder(long FolderType);
	BOOL GetIsInstantSearchEnabled();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _Rules wrapper class

	class _Rules : public COleDispatchDriver
	{
	public:
	_Rules() {}		// Calls COleDispatchDriver default constructor
	_Rules(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Rules(const _Rules& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH Create(LPCTSTR Name, long RuleType);
	void Save(const VARIANT& ShowProgress);
	void Remove(const VARIANT& Index);
	BOOL GetIsRssRulesProcessingEnabled();
	void SetIsRssRulesProcessingEnabled(BOOL bNewValue);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _Rule wrapper class

	class _Rule : public COleDispatchDriver
	{
	public:
	_Rule() {}		// Calls COleDispatchDriver default constructor
	_Rule(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Rule(const _Rule& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	long GetExecutionOrder();
	void SetExecutionOrder(long nNewValue);
	long GetRuleType();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	BOOL GetIsLocalRule();
	void Execute(const VARIANT& ShowProgress, const VARIANT& Folder, const VARIANT& IncludeSubfolders, const VARIANT& RuleExecuteOption);
	LPDISPATCH GetActions();
	LPDISPATCH GetConditions();
	LPDISPATCH GetExceptions();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _RuleActions wrapper class

	class _RuleActions : public COleDispatchDriver
	{
	public:
	_RuleActions() {}		// Calls COleDispatchDriver default constructor
	_RuleActions(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_RuleActions(const _RuleActions& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(long Index);
	LPDISPATCH GetCopyToFolder();
	LPDISPATCH GetDeletePermanently();
	LPDISPATCH GetDelete();
	LPDISPATCH GetDesktopAlert();
	LPDISPATCH GetNotifyDelivery();
	LPDISPATCH GetNotifyRead();
	LPDISPATCH GetStop();
	LPDISPATCH GetMoveToFolder();
	LPDISPATCH GetCc();
	LPDISPATCH GetForward();
	LPDISPATCH GetForwardAsAttachment();
	LPDISPATCH GetRedirect();
	LPDISPATCH GetAssignToCategory();
	LPDISPATCH GetPlaySound();
	LPDISPATCH GetMarkAsTask();
	LPDISPATCH GetNewItemAlert();
	LPDISPATCH GetClearCategories();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _RuleAction wrapper class

	class _RuleAction : public COleDispatchDriver
	{
	public:
	_RuleAction() {}		// Calls COleDispatchDriver default constructor
	_RuleAction(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_RuleAction(const _RuleAction& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	long GetActionType();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _MoveOrCopyRuleAction wrapper class

	class _MoveOrCopyRuleAction : public COleDispatchDriver
	{
	public:
	_MoveOrCopyRuleAction() {}		// Calls COleDispatchDriver default constructor
	_MoveOrCopyRuleAction(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_MoveOrCopyRuleAction(const _MoveOrCopyRuleAction& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	long GetActionType();
	LPDISPATCH GetFolder();
	void SetFolder(LPDISPATCH newValue);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _SendRuleAction wrapper class

	class _SendRuleAction : public COleDispatchDriver
	{
	public:
	_SendRuleAction() {}		// Calls COleDispatchDriver default constructor
	_SendRuleAction(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_SendRuleAction(const _SendRuleAction& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	long GetActionType();
	LPDISPATCH GetRecipients();
	};
	/////////////////////////////////////////////////////////////////////////////
	// Recipients wrapper class

	class Recipients : public COleDispatchDriver
	{
	public:
	Recipients() {}		// Calls COleDispatchDriver default constructor
	Recipients(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Recipients(const Recipients& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH Add(LPCTSTR Name);
	void Remove(long Index);
	BOOL ResolveAll();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _AssignToCategoryRuleAction wrapper class

	class _AssignToCategoryRuleAction : public COleDispatchDriver
	{
	public:
	_AssignToCategoryRuleAction() {}		// Calls COleDispatchDriver default constructor
	_AssignToCategoryRuleAction(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_AssignToCategoryRuleAction(const _AssignToCategoryRuleAction& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	long GetActionType();
	VARIANT GetCategories();
	void SetCategories(const VARIANT& newValue);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _PlaySoundRuleAction wrapper class

	class _PlaySoundRuleAction : public COleDispatchDriver
	{
	public:
	_PlaySoundRuleAction() {}		// Calls COleDispatchDriver default constructor
	_PlaySoundRuleAction(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_PlaySoundRuleAction(const _PlaySoundRuleAction& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	long GetActionType();
	CString GetFilePath();
	void SetFilePath(LPCTSTR lpszNewValue);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _MarkAsTaskRuleAction wrapper class

	class _MarkAsTaskRuleAction : public COleDispatchDriver
	{
	public:
	_MarkAsTaskRuleAction() {}		// Calls COleDispatchDriver default constructor
	_MarkAsTaskRuleAction(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_MarkAsTaskRuleAction(const _MarkAsTaskRuleAction& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	long GetActionType();
	CString GetFlagTo();
	void SetFlagTo(LPCTSTR lpszNewValue);
	long GetMarkInterval();
	void SetMarkInterval(long nNewValue);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _NewItemAlertRuleAction wrapper class

	class _NewItemAlertRuleAction : public COleDispatchDriver
	{
	public:
	_NewItemAlertRuleAction() {}		// Calls COleDispatchDriver default constructor
	_NewItemAlertRuleAction(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_NewItemAlertRuleAction(const _NewItemAlertRuleAction& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	long GetActionType();
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _RuleConditions wrapper class

	class _RuleConditions : public COleDispatchDriver
	{
	public:
	_RuleConditions() {}		// Calls COleDispatchDriver default constructor
	_RuleConditions(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_RuleConditions(const _RuleConditions& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(long Index);
	LPDISPATCH GetCc();
	LPDISPATCH GetHasAttachment();
	LPDISPATCH GetImportance();
	LPDISPATCH GetMeetingInviteOrUpdate();
	LPDISPATCH GetNotTo();
	LPDISPATCH GetOnlyToMe();
	LPDISPATCH GetToMe();
	LPDISPATCH GetToOrCc();
	LPDISPATCH GetAccount();
	LPDISPATCH GetBody();
	LPDISPATCH GetBodyOrSubject();
	LPDISPATCH GetCategory();
	LPDISPATCH GetFormName();
	LPDISPATCH GetFrom();
	LPDISPATCH GetMessageHeader();
	LPDISPATCH GetRecipientAddress();
	LPDISPATCH GetSenderAddress();
	LPDISPATCH GetSenderInAddressList();
	LPDISPATCH GetSubject();
	LPDISPATCH GetSentTo();
	LPDISPATCH GetOnLocalMachine();
	LPDISPATCH GetOnOtherMachine();
	LPDISPATCH GetAnyCategory();
	LPDISPATCH GetFromAnyRSSFeed();
	LPDISPATCH GetFromRssFeed();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _RuleCondition wrapper class

	class _RuleCondition : public COleDispatchDriver
	{
	public:
	_RuleCondition() {}		// Calls COleDispatchDriver default constructor
	_RuleCondition(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_RuleCondition(const _RuleCondition& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	long GetConditionType();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _ImportanceRuleCondition wrapper class

	class _ImportanceRuleCondition : public COleDispatchDriver
	{
	public:
	_ImportanceRuleCondition() {}		// Calls COleDispatchDriver default constructor
	_ImportanceRuleCondition(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_ImportanceRuleCondition(const _ImportanceRuleCondition& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	long GetConditionType();
	long GetImportance();
	void SetImportance(long nNewValue);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _AccountRuleCondition wrapper class

	class _AccountRuleCondition : public COleDispatchDriver
	{
	public:
	_AccountRuleCondition() {}		// Calls COleDispatchDriver default constructor
	_AccountRuleCondition(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_AccountRuleCondition(const _AccountRuleCondition& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	long GetConditionType();
	LPDISPATCH GetAccount();
	void SetAccount(LPDISPATCH newValue);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _Account wrapper class

	class _Account : public COleDispatchDriver
	{
	public:
	_Account() {}		// Calls COleDispatchDriver default constructor
	_Account(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Account(const _Account& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetAccountType();
	CString GetDisplayName();
	CString GetUserName_();
	CString GetSmtpAddress();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _TextRuleCondition wrapper class

	class _TextRuleCondition : public COleDispatchDriver
	{
	public:
	_TextRuleCondition() {}		// Calls COleDispatchDriver default constructor
	_TextRuleCondition(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_TextRuleCondition(const _TextRuleCondition& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	long GetConditionType();
	VARIANT GetText();
	void SetText(const VARIANT& newValue);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _CategoryRuleCondition wrapper class

	class _CategoryRuleCondition : public COleDispatchDriver
	{
	public:
	_CategoryRuleCondition() {}		// Calls COleDispatchDriver default constructor
	_CategoryRuleCondition(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_CategoryRuleCondition(const _CategoryRuleCondition& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	long GetConditionType();
	VARIANT GetCategories();
	void SetCategories(const VARIANT& newValue);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _FormNameRuleCondition wrapper class

	class _FormNameRuleCondition : public COleDispatchDriver
	{
	public:
	_FormNameRuleCondition() {}		// Calls COleDispatchDriver default constructor
	_FormNameRuleCondition(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_FormNameRuleCondition(const _FormNameRuleCondition& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	long GetConditionType();
	VARIANT GetFormName();
	void SetFormName(const VARIANT& newValue);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _ToOrFromRuleCondition wrapper class

	class _ToOrFromRuleCondition : public COleDispatchDriver
	{
	public:
	_ToOrFromRuleCondition() {}		// Calls COleDispatchDriver default constructor
	_ToOrFromRuleCondition(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_ToOrFromRuleCondition(const _ToOrFromRuleCondition& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	long GetConditionType();
	LPDISPATCH GetRecipients();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _AddressRuleCondition wrapper class

	class _AddressRuleCondition : public COleDispatchDriver
	{
	public:
	_AddressRuleCondition() {}		// Calls COleDispatchDriver default constructor
	_AddressRuleCondition(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_AddressRuleCondition(const _AddressRuleCondition& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	long GetConditionType();
	VARIANT GetAddress();
	void SetAddress(const VARIANT& newValue);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _SenderInAddressListRuleCondition wrapper class

	class _SenderInAddressListRuleCondition : public COleDispatchDriver
	{
	public:
	_SenderInAddressListRuleCondition() {}		// Calls COleDispatchDriver default constructor
	_SenderInAddressListRuleCondition(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_SenderInAddressListRuleCondition(const _SenderInAddressListRuleCondition& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	long GetConditionType();
	LPDISPATCH GetAddressList();
	void SetAddressList(LPDISPATCH newValue);
	};
	/////////////////////////////////////////////////////////////////////////////
	// AddressList wrapper class

	class AddressList : public COleDispatchDriver
	{
	public:
	AddressList() {}		// Calls COleDispatchDriver default constructor
	AddressList(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	AddressList(const AddressList& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	LPDISPATCH GetAddressEntries();
	CString GetId();
	long GetIndex();
	BOOL GetIsReadOnly();
	CString GetName();
	LPDISPATCH GetContactsFolder();
	long GetAddressListType();
	long GetResolutionOrder();
	BOOL GetIsInitialAddressList();
	LPDISPATCH GetPropertyAccessor();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _FromRssFeedRuleCondition wrapper class

	class _FromRssFeedRuleCondition : public COleDispatchDriver
	{
	public:
	_FromRssFeedRuleCondition() {}		// Calls COleDispatchDriver default constructor
	_FromRssFeedRuleCondition(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_FromRssFeedRuleCondition(const _FromRssFeedRuleCondition& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	long GetConditionType();
	VARIANT GetFromRssFeed();
	void SetFromRssFeed(const VARIANT& newValue);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _StorageItem wrapper class

	class _StorageItem : public COleDispatchDriver
	{
	public:
	_StorageItem() {}		// Calls COleDispatchDriver default constructor
	_StorageItem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_StorageItem(const _StorageItem& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	DATE GetCreationTime();
	DATE GetLastModificationTime();
	CString GetEntryID();
	void Save();
	void Delete();
	CString GetBody();
	void SetBody(LPCTSTR lpszNewValue);
	LPDISPATCH GetAttachments();
	long GetSize();
	LPDISPATCH GetUserProperties();
	LPDISPATCH GetPropertyAccessor();
	CString GetSubject();
	void SetSubject(LPCTSTR lpszNewValue);
	CString GetCreator();
	void SetCreator(LPCTSTR lpszNewValue);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _Table wrapper class

	class _Table : public COleDispatchDriver
	{
	public:
	_Table() {}		// Calls COleDispatchDriver default constructor
	_Table(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Table(const _Table& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	LPDISPATCH FindRow(LPCTSTR Filter);
	LPDISPATCH FindNextRow();
	VARIANT GetArray(long MaxRows);
	LPDISPATCH GetNextRow();
	long GetRowCount();
	void MoveToStart();
	LPDISPATCH Restrict(LPCTSTR Filter);
	void Sort(LPCTSTR SortProperty, const VARIANT& Descending);
	LPDISPATCH GetColumns();
	BOOL GetEndOfTable();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _Row wrapper class

	class _Row : public COleDispatchDriver
	{
	public:
	_Row() {}		// Calls COleDispatchDriver default constructor
	_Row(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Row(const _Row& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	CString BinaryToString(const VARIANT& Index);
	VARIANT GetValues();
	VARIANT Item(const VARIANT& Index);
	DATE LocalTimeToUTC(const VARIANT& Index);
	DATE UTCToLocalTime(const VARIANT& Index);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _Columns wrapper class

	class _Columns : public COleDispatchDriver
	{
	public:
	_Columns() {}		// Calls COleDispatchDriver default constructor
	_Columns(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Columns(const _Columns& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH Add(LPCTSTR Name);
	void Remove(const VARIANT& Index);
	void RemoveAll();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _Column wrapper class

	class _Column : public COleDispatchDriver
	{
	public:
	_Column() {}		// Calls COleDispatchDriver default constructor
	_Column(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Column(const _Column& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	CString GetName();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _CalendarSharing wrapper class

	class _CalendarSharing : public COleDispatchDriver
	{
	public:
	_CalendarSharing() {}		// Calls COleDispatchDriver default constructor
	_CalendarSharing(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_CalendarSharing(const _CalendarSharing& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	void SaveAsICal(LPCTSTR Path);
	LPDISPATCH ForwardAsICal(long MailFormat);
	long GetCalendarDetail();
	void SetCalendarDetail(long nNewValue);
	DATE GetEndDate();
	void SetEndDate(DATE newValue);
	LPDISPATCH GetFolder();
	BOOL GetIncludeAttachments();
	void SetIncludeAttachments(BOOL bNewValue);
	BOOL GetIncludePrivateDetails();
	void SetIncludePrivateDetails(BOOL bNewValue);
	BOOL GetRestrictToWorkingHours();
	void SetRestrictToWorkingHours(BOOL bNewValue);
	DATE GetStartDate();
	void SetStartDate(DATE newValue);
	BOOL GetIncludeWholeCalendar();
	void SetIncludeWholeCalendar(BOOL bNewValue);
	};
	*/

	/////////////////////////////////////////////////////////////////////////////
	// Links wrapper class
	/*
	class Links : public COleDispatchDriver
	{
	public:
	Links() {}		// Calls COleDispatchDriver default constructor
	Links(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Links(const Links& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH Add(LPDISPATCH Item);
	void Remove(const VARIANT& Index);
	};
	/////////////////////////////////////////////////////////////////////////////
	// Link wrapper class

	class Link : public COleDispatchDriver
	{
	public:
	Link() {}		// Calls COleDispatchDriver default constructor
	Link(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Link(const Link& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	CString GetName();
	long GetType();
	LPDISPATCH GetItem();
	};
	/////////////////////////////////////////////////////////////////////////////
	// ItemProperties wrapper class

	class ItemProperties : public COleDispatchDriver
	{
	public:
	ItemProperties() {}		// Calls COleDispatchDriver default constructor
	ItemProperties(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	ItemProperties(const ItemProperties& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH Add(LPCTSTR Name, long Type, const VARIANT& AddToFolderFields, const VARIANT& DisplayFormat);
	void Remove(long Index);
	};
	/////////////////////////////////////////////////////////////////////////////
	// ItemProperty wrapper class

	class ItemProperty : public COleDispatchDriver
	{
	public:
	ItemProperty() {}		// Calls COleDispatchDriver default constructor
	ItemProperty(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	ItemProperty(const ItemProperty& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	CString GetName();
	long GetType();
	VARIANT GetValue();
	void SetValue(const VARIANT& newValue);
	BOOL GetIsUserProperty();
	};
	/////////////////////////////////////////////////////////////////////////////
	// Conflicts wrapper class

	class Conflicts : public COleDispatchDriver
	{
	public:
	Conflicts() {}		// Calls COleDispatchDriver default constructor
	Conflicts(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Conflicts(const Conflicts& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH GetFirst();
	LPDISPATCH GetLast();
	LPDISPATCH GetNext();
	LPDISPATCH GetPrevious();
	};
	/////////////////////////////////////////////////////////////////////////////
	// Conflict wrapper class

	class Conflict : public COleDispatchDriver
	{
	public:
	Conflict() {}		// Calls COleDispatchDriver default constructor
	Conflict(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Conflict(const Conflict& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	LPDISPATCH GetItem();
	CString GetName();
	long GetType();
	};
	/////////////////////////////////////////////////////////////////////////////
	// ItemEvents wrapper class

	class ItemEvents : public COleDispatchDriver
	{
	public:
	ItemEvents() {}		// Calls COleDispatchDriver default constructor
	ItemEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	ItemEvents(const ItemEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	void Open(BOOL* Cancel);
	void CustomAction(LPDISPATCH Action, LPDISPATCH Response, BOOL* Cancel);
	void CustomPropertyChange(LPCTSTR Name);
	void Forward(LPDISPATCH Forward, BOOL* Cancel);
	void Close(BOOL* Cancel);
	void PropertyChange(LPCTSTR Name);
	void Read();
	void Reply(LPDISPATCH Response, BOOL* Cancel);
	void ReplyAll(LPDISPATCH Response, BOOL* Cancel);
	void Send(BOOL* Cancel);
	void Write(BOOL* Cancel);
	void BeforeCheckNames(BOOL* Cancel);
	void AttachmentAdd(LPDISPATCH Attachment);
	void AttachmentRead(LPDISPATCH Attachment);
	void BeforeAttachmentSave(LPDISPATCH Attachment, BOOL* Cancel);
	};
	/////////////////////////////////////////////////////////////////////////////
	// ItemEvents_10 wrapper class

	class ItemEvents_10 : public COleDispatchDriver
	{
	public:
	ItemEvents_10() {}		// Calls COleDispatchDriver default constructor
	ItemEvents_10(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	ItemEvents_10(const ItemEvents_10& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	// method 'Open' not emitted because of invalid return type or parameter type
	// method 'CustomAction' not emitted because of invalid return type or parameter type
	// method 'CustomPropertyChange' not emitted because of invalid return type or parameter type
	// method 'Forward' not emitted because of invalid return type or parameter type
	// method 'Close' not emitted because of invalid return type or parameter type
	// method 'PropertyChange' not emitted because of invalid return type or parameter type
	// method 'Read' not emitted because of invalid return type or parameter type
	// method 'Reply' not emitted because of invalid return type or parameter type
	// method 'ReplyAll' not emitted because of invalid return type or parameter type
	// method 'Send' not emitted because of invalid return type or parameter type
	// method 'Write' not emitted because of invalid return type or parameter type
	// method 'BeforeCheckNames' not emitted because of invalid return type or parameter type
	// method 'AttachmentAdd' not emitted because of invalid return type or parameter type
	// method 'AttachmentRead' not emitted because of invalid return type or parameter type
	// method 'BeforeAttachmentSave' not emitted because of invalid return type or parameter type
	void BeforeDelete(LPDISPATCH Item, BOOL* Cancel);
	void AttachmentRemove(LPDISPATCH Attachment);
	void BeforeAttachmentAdd(LPDISPATCH Attachment, BOOL* Cancel);
	void BeforeAttachmentPreview(LPDISPATCH Attachment, BOOL* Cancel);
	void BeforeAttachmentRead(LPDISPATCH Attachment, BOOL* Cancel);
	void BeforeAttachmentWriteToTempFile(LPDISPATCH Attachment, BOOL* Cancel);
	void Unload();
	void BeforeAutoSave(BOOL* Cancel);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _UserDefinedProperties wrapper class

	class _UserDefinedProperties : public COleDispatchDriver
	{
	public:
	_UserDefinedProperties() {}		// Calls COleDispatchDriver default constructor
	_UserDefinedProperties(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_UserDefinedProperties(const _UserDefinedProperties& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH Add(LPCTSTR Name, long Type, const VARIANT& DisplayFormat, const VARIANT& Formula);
	LPDISPATCH Find(LPCTSTR Name);
	void Remove(long Index);
	void Refresh();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _UserDefinedProperty wrapper class

	class _UserDefinedProperty : public COleDispatchDriver
	{
	public:
	_UserDefinedProperty() {}		// Calls COleDispatchDriver default constructor
	_UserDefinedProperty(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_UserDefinedProperty(const _UserDefinedProperty& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	CString GetFormula();
	CString GetName();
	long GetType();
	long GetDisplayFormat();
	void Delete();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _ExchangeUser wrapper class

	class _ExchangeUser : public COleDispatchDriver
	{
	public:
	_ExchangeUser() {}		// Calls COleDispatchDriver default constructor
	_ExchangeUser(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_ExchangeUser(const _ExchangeUser& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	CString GetAddress();
	void SetAddress(LPCTSTR lpszNewValue);
	long GetDisplayType();
	CString GetId();
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	CString GetType();
	void SetType(LPCTSTR lpszNewValue);
	void Delete();
	void Details(const VARIANT& HWnd);
	CString GetFreeBusy(DATE Start, long MinPerChar, const VARIANT& CompleteFormat);
	void Update(const VARIANT& MakePermanent, const VARIANT& Refresh);
	LPDISPATCH GetContact();
	LPDISPATCH GetExchangeUser();
	long GetAddressEntryUserType();
	LPDISPATCH GetExchangeDistributionList();
	LPDISPATCH GetPropertyAccessor();
	LPDISPATCH GetDirectReports();
	LPDISPATCH GetMemberOfList();
	CString GetAlias();
	CString GetAssistantName();
	void SetAssistantName(LPCTSTR lpszNewValue);
	CString GetBusinessTelephoneNumber();
	void SetBusinessTelephoneNumber(LPCTSTR lpszNewValue);
	CString GetCity();
	void SetCity(LPCTSTR lpszNewValue);
	CString GetComments();
	void SetComments(LPCTSTR lpszNewValue);
	CString GetCompanyName();
	void SetCompanyName(LPCTSTR lpszNewValue);
	CString GetDepartment();
	void SetDepartment(LPCTSTR lpszNewValue);
	CString GetFirstName();
	void SetFirstName(LPCTSTR lpszNewValue);
	CString GetJobTitle();
	void SetJobTitle(LPCTSTR lpszNewValue);
	CString GetLastName();
	void SetLastName(LPCTSTR lpszNewValue);
	CString GetMobileTelephoneNumber();
	void SetMobileTelephoneNumber(LPCTSTR lpszNewValue);
	CString GetOfficeLocation();
	void SetOfficeLocation(LPCTSTR lpszNewValue);
	CString GetPostalCode();
	void SetPostalCode(LPCTSTR lpszNewValue);
	CString GetPrimarySmtpAddress();
	CString GetStateOrProvince();
	void SetStateOrProvince(LPCTSTR lpszNewValue);
	CString GetStreetAddress();
	void SetStreetAddress(LPCTSTR lpszNewValue);
	LPDISPATCH GetExchangeUserManager();
	CString GetYomiCompanyName();
	void SetYomiCompanyName(LPCTSTR lpszNewValue);
	CString GetYomiFirstName();
	void SetYomiFirstName(LPCTSTR lpszNewValue);
	CString GetYomiLastName();
	void SetYomiLastName(LPCTSTR lpszNewValue);
	CString GetYomiDisplayName();
	void SetYomiDisplayName(LPCTSTR lpszNewValue);
	CString GetYomiDepartment();
	void SetYomiDepartment(LPCTSTR lpszNewValue);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _ExchangeDistributionList wrapper class

	class _ExchangeDistributionList : public COleDispatchDriver
	{
	public:
	_ExchangeDistributionList() {}		// Calls COleDispatchDriver default constructor
	_ExchangeDistributionList(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_ExchangeDistributionList(const _ExchangeDistributionList& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	CString GetAddress();
	void SetAddress(LPCTSTR lpszNewValue);
	long GetDisplayType();
	CString GetId();
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	CString GetType();
	void SetType(LPCTSTR lpszNewValue);
	void Delete();
	void Details(const VARIANT& HWnd);
	CString GetFreeBusy(DATE Start, long MinPerChar, const VARIANT& CompleteFormat);
	void Update(const VARIANT& MakePermanent, const VARIANT& Refresh);
	LPDISPATCH GetContact();
	LPDISPATCH GetExchangeUser();
	long GetAddressEntryUserType();
	LPDISPATCH GetExchangeDistributionList();
	LPDISPATCH GetPropertyAccessor();
	LPDISPATCH GetMemberOfList();
	LPDISPATCH GetExchangeDistributionListMembers();
	CString GetAlias();
	CString GetComments();
	void SetComments(LPCTSTR lpszNewValue);
	CString GetPrimarySmtpAddress();
	LPDISPATCH GetOwners();
	};
	/////////////////////////////////////////////////////////////////////////////
	// AddressLists wrapper class

	class AddressLists : public COleDispatchDriver
	{
	public:
	AddressLists() {}		// Calls COleDispatchDriver default constructor
	AddressLists(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	AddressLists(const AddressLists& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	};
	/////////////////////////////////////////////////////////////////////////////
	// SyncObjects wrapper class

	class SyncObjects : public COleDispatchDriver
	{
	public:
	SyncObjects() {}		// Calls COleDispatchDriver default constructor
	SyncObjects(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	SyncObjects(const SyncObjects& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH GetAppFolders();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _SyncObject wrapper class

	class _SyncObject : public COleDispatchDriver
	{
	public:
	_SyncObject() {}		// Calls COleDispatchDriver default constructor
	_SyncObject(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_SyncObject(const _SyncObject& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	CString GetName();
	void Start();
	void Stop();
	};
	/////////////////////////////////////////////////////////////////////////////
	// SyncObjectEvents wrapper class

	class SyncObjectEvents : public COleDispatchDriver
	{
	public:
	SyncObjectEvents() {}		// Calls COleDispatchDriver default constructor
	SyncObjectEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	SyncObjectEvents(const SyncObjectEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	void SyncStart();
	void Progress(long State, LPCTSTR Description, long Value, long Max);
	void OnError(long Code, LPCTSTR Description);
	void SyncEnd();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _Accounts wrapper class

	class _Accounts : public COleDispatchDriver
	{
	public:
	_Accounts() {}		// Calls COleDispatchDriver default constructor
	_Accounts(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Accounts(const _Accounts& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _Stores wrapper class

	class _Stores : public COleDispatchDriver
	{
	public:
	_Stores() {}		// Calls COleDispatchDriver default constructor
	_Stores(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Stores(const _Stores& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	};
	/////////////////////////////////////////////////////////////////////////////
	// StoresEvents_12 wrapper class

	class StoresEvents_12 : public COleDispatchDriver
	{
	public:
	StoresEvents_12() {}		// Calls COleDispatchDriver default constructor
	StoresEvents_12(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	StoresEvents_12(const StoresEvents_12& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	void BeforeStoreRemove(LPDISPATCH Store, BOOL* Cancel);
	void StoreAdd(LPDISPATCH Store);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _SelectNamesDialog wrapper class

	class _SelectNamesDialog : public COleDispatchDriver
	{
	public:
	_SelectNamesDialog() {}		// Calls COleDispatchDriver default constructor
	_SelectNamesDialog(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_SelectNamesDialog(const _SelectNamesDialog& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	BOOL Display();
	LPDISPATCH GetRecipients();
	void SetRecipients(LPDISPATCH newValue);
	CString GetBccLabel();
	void SetBccLabel(LPCTSTR lpszNewValue);
	CString GetCcLabel();
	void SetCcLabel(LPCTSTR lpszNewValue);
	CString GetToLabel();
	void SetToLabel(LPCTSTR lpszNewValue);
	BOOL GetAllowMultipleSelection();
	void SetAllowMultipleSelection(BOOL bNewValue);
	BOOL GetForceResolution();
	void SetForceResolution(BOOL bNewValue);
	BOOL GetShowOnlyInitialAddressList();
	void SetShowOnlyInitialAddressList(BOOL bNewValue);
	long GetNumberOfRecipientSelectors();
	void SetNumberOfRecipientSelectors(long nNewValue);
	LPDISPATCH GetInitialAddressList();
	void SetInitialAddressList(LPDISPATCH newValue);
	void SetDefaultDisplayMode(long defaultMode);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _Categories wrapper class

	class _Categories : public COleDispatchDriver
	{
	public:
	_Categories() {}		// Calls COleDispatchDriver default constructor
	_Categories(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Categories(const _Categories& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH Add(LPCTSTR Name, const VARIANT& Color, const VARIANT& ShortcutKey);
	void Remove(const VARIANT& Index);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _Category wrapper class

	class _Category : public COleDispatchDriver
	{
	public:
	_Category() {}		// Calls COleDispatchDriver default constructor
	_Category(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Category(const _Category& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	long GetColor();
	void SetColor(long nNewValue);
	long GetShortcutKey();
	void SetShortcutKey(long nNewValue);
	CString GetCategoryID();
	unsigned long GetCategoryBorderColor();
	unsigned long GetCategoryGradientTopColor();
	unsigned long GetCategoryGradientBottomColor();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _SharingItem wrapper class

	class _SharingItem : public COleDispatchDriver
	{
	public:
	_SharingItem() {}		// Calls COleDispatchDriver default constructor
	_SharingItem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_SharingItem(const _SharingItem& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	LPDISPATCH GetActions();
	LPDISPATCH GetAttachments();
	CString GetBillingInformation();
	void SetBillingInformation(LPCTSTR lpszNewValue);
	CString GetBody();
	void SetBody(LPCTSTR lpszNewValue);
	CString GetCategories();
	void SetCategories(LPCTSTR lpszNewValue);
	CString GetCompanies();
	void SetCompanies(LPCTSTR lpszNewValue);
	CString GetConversationIndex();
	CString GetConversationTopic();
	DATE GetCreationTime();
	CString GetEntryID();
	LPDISPATCH GetFormDescription();
	LPDISPATCH GetGetInspector();
	long GetImportance();
	void SetImportance(long nNewValue);
	DATE GetLastModificationTime();
	CString GetMessageClass();
	void SetMessageClass(LPCTSTR lpszNewValue);
	CString GetMileage();
	void SetMileage(LPCTSTR lpszNewValue);
	BOOL GetNoAging();
	void SetNoAging(BOOL bNewValue);
	long GetOutlookInternalVersion();
	CString GetOutlookVersion();
	BOOL GetSaved();
	long GetSensitivity();
	void SetSensitivity(long nNewValue);
	long GetSize();
	CString GetSubject();
	void SetSubject(LPCTSTR lpszNewValue);
	BOOL GetUnRead();
	void SetUnRead(BOOL bNewValue);
	LPDISPATCH GetUserProperties();
	void Close(long SaveMode);
	LPDISPATCH Copy();
	void Delete();
	void Display(const VARIANT& Modal);
	LPDISPATCH Move(LPDISPATCH DestFldr);
	void PrintOut();
	void Save();
	void SaveAs(LPCTSTR Path, const VARIANT& Type);
	void Allow();
	LPDISPATCH Deny();
	LPDISPATCH GetPropertyAccessor();
	CString GetRemoteName();
	CString GetRemoteID();
	CString GetRemotePath();
	CString GetSharingProviderGuid();
	LPDISPATCH OpenSharedFolder();
	long GetSharingProvider();
	BOOL GetAllowWriteAccess();
	void SetAllowWriteAccess(BOOL bNewValue);
	long GetType();
	void SetType(long nNewValue);
	long GetRequestedFolder();
	LPDISPATCH GetSendUsingAccount();
	void SetSendUsingAccount(LPDISPATCH newValue);
	BOOL GetAlternateRecipientAllowed();
	void SetAlternateRecipientAllowed(BOOL bNewValue);
	BOOL GetAutoForwarded();
	void SetAutoForwarded(BOOL bNewValue);
	CString GetBcc();
	void SetBcc(LPCTSTR lpszNewValue);
	CString GetCc();
	void SetCc(LPCTSTR lpszNewValue);
	DATE GetDeferredDeliveryTime();
	void SetDeferredDeliveryTime(DATE newValue);
	BOOL GetDeleteAfterSubmit();
	void SetDeleteAfterSubmit(BOOL bNewValue);
	DATE GetExpiryTime();
	void SetExpiryTime(DATE newValue);
	CString GetFlagRequest();
	void SetFlagRequest(LPCTSTR lpszNewValue);
	CString GetHTMLBody();
	void SetHTMLBody(LPCTSTR lpszNewValue);
	BOOL GetOriginatorDeliveryReportRequested();
	void SetOriginatorDeliveryReportRequested(BOOL bNewValue);
	BOOL GetReadReceiptRequested();
	void SetReadReceiptRequested(BOOL bNewValue);
	CString GetReceivedByEntryID();
	CString GetReceivedByName();
	CString GetReceivedOnBehalfOfEntryID();
	CString GetReceivedOnBehalfOfName();
	DATE GetReceivedTime();
	BOOL GetRecipientReassignmentProhibited();
	void SetRecipientReassignmentProhibited(BOOL bNewValue);
	LPDISPATCH GetRecipients();
	BOOL GetReminderOverrideDefault();
	void SetReminderOverrideDefault(BOOL bNewValue);
	BOOL GetReminderPlaySound();
	void SetReminderPlaySound(BOOL bNewValue);
	BOOL GetReminderSet();
	void SetReminderSet(BOOL bNewValue);
	CString GetReminderSoundFile();
	void SetReminderSoundFile(LPCTSTR lpszNewValue);
	DATE GetReminderTime();
	void SetReminderTime(DATE newValue);
	long GetRemoteStatus();
	void SetRemoteStatus(long nNewValue);
	CString GetReplyRecipientNames();
	LPDISPATCH GetReplyRecipients();
	LPDISPATCH GetSaveSentMessageFolder();
	void SetRefSaveSentMessageFolder(LPDISPATCH newValue);
	CString GetSenderName();
	BOOL GetSent();
	DATE GetSentOn();
	CString GetSentOnBehalfOfName();
	void SetSentOnBehalfOfName(LPCTSTR lpszNewValue);
	BOOL GetSubmitted();
	CString GetTo();
	void SetTo(LPCTSTR lpszNewValue);
	void ClearConversationIndex();
	LPDISPATCH Forward();
	LPDISPATCH Reply();
	LPDISPATCH ReplyAll();
	void Send();
	LPDISPATCH GetItemProperties();
	long GetBodyFormat();
	void SetBodyFormat(long nNewValue);
	long GetDownloadState();
	void ShowCategoriesDialog();
	long GetInternetCodepage();
	void SetInternetCodepage(long nNewValue);
	long GetMarkForDownload();
	void SetMarkForDownload(long nNewValue);
	BOOL GetIsConflict();
	void AddBusinessCard(LPDISPATCH contact);
	CString GetTaskSubject();
	void SetTaskSubject(LPCTSTR lpszNewValue);
	DATE GetTaskDueDate();
	void SetTaskDueDate(DATE newValue);
	DATE GetTaskStartDate();
	void SetTaskStartDate(DATE newValue);
	DATE GetTaskCompletedDate();
	void SetTaskCompletedDate(DATE newValue);
	DATE GetToDoTaskOrdinal();
	void SetToDoTaskOrdinal(DATE newValue);
	LPDISPATCH GetConflicts();
	CString GetSenderEmailAddress();
	CString GetSenderEmailType();
	long GetPermission();
	void SetPermission(long nNewValue);
	long GetPermissionService();
	void SetPermissionService(long nNewValue);
	void MarkAsTask(long MarkInterval);
	void ClearTaskFlag();
	BOOL GetIsMarkedAsTask();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _Explorers wrapper class

	class _Explorers : public COleDispatchDriver
	{
	public:
	_Explorers() {}		// Calls COleDispatchDriver default constructor
	_Explorers(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Explorers(const _Explorers& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH Add(const VARIANT& Folder, long DisplayMode);
	};
	/////////////////////////////////////////////////////////////////////////////
	// ExplorerEvents wrapper class

	class ExplorerEvents : public COleDispatchDriver
	{
	public:
	ExplorerEvents() {}		// Calls COleDispatchDriver default constructor
	ExplorerEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	ExplorerEvents(const ExplorerEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	void Activate();
	void FolderSwitch();
	void BeforeFolderSwitch(LPDISPATCH NewFolder, BOOL* Cancel);
	void ViewSwitch();
	void BeforeViewSwitch(const VARIANT& NewView, BOOL* Cancel);
	void Deactivate();
	void SelectionChange();
	void Close();
	};
	/////////////////////////////////////////////////////////////////////////////
	// ExplorerEvents_10 wrapper class

	class ExplorerEvents_10 : public COleDispatchDriver
	{
	public:
	ExplorerEvents_10() {}		// Calls COleDispatchDriver default constructor
	ExplorerEvents_10(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	ExplorerEvents_10(const ExplorerEvents_10& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	// method 'Activate' not emitted because of invalid return type or parameter type
	// method 'FolderSwitch' not emitted because of invalid return type or parameter type
	// method 'BeforeFolderSwitch' not emitted because of invalid return type or parameter type
	// method 'ViewSwitch' not emitted because of invalid return type or parameter type
	// method 'BeforeViewSwitch' not emitted because of invalid return type or parameter type
	// method 'Deactivate' not emitted because of invalid return type or parameter type
	// method 'SelectionChange' not emitted because of invalid return type or parameter type
	// method 'Close' not emitted because of invalid return type or parameter type
	// method 'BeforeMaximize' not emitted because of invalid return type or parameter type
	// method 'BeforeMinimize' not emitted because of invalid return type or parameter type
	// method 'BeforeMove' not emitted because of invalid return type or parameter type
	// method 'BeforeSize' not emitted because of invalid return type or parameter type
	void BeforeItemCopy(BOOL* Cancel);
	void BeforeItemCut(BOOL* Cancel);
	void BeforeItemPaste(VARIANT* ClipboardContent, LPDISPATCH Target, BOOL* Cancel);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _Inspectors wrapper class

	class _Inspectors : public COleDispatchDriver
	{
	public:
	_Inspectors() {}		// Calls COleDispatchDriver default constructor
	_Inspectors(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Inspectors(const _Inspectors& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH Add(LPDISPATCH Item);
	};
	/////////////////////////////////////////////////////////////////////////////
	// InspectorEvents wrapper class

	class InspectorEvents : public COleDispatchDriver
	{
	public:
	InspectorEvents() {}		// Calls COleDispatchDriver default constructor
	InspectorEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	InspectorEvents(const InspectorEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	void Activate();
	void Deactivate();
	void Close();
	};
	/////////////////////////////////////////////////////////////////////////////
	// InspectorEvents_10 wrapper class

	class InspectorEvents_10 : public COleDispatchDriver
	{
	public:
	InspectorEvents_10() {}		// Calls COleDispatchDriver default constructor
	InspectorEvents_10(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	InspectorEvents_10(const InspectorEvents_10& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	// method 'Activate' not emitted because of invalid return type or parameter type
	// method 'Deactivate' not emitted because of invalid return type or parameter type
	// method 'Close' not emitted because of invalid return type or parameter type
	// method 'BeforeMaximize' not emitted because of invalid return type or parameter type
	// method 'BeforeMinimize' not emitted because of invalid return type or parameter type
	// method 'BeforeMove' not emitted because of invalid return type or parameter type
	// method 'BeforeSize' not emitted because of invalid return type or parameter type
	void PageChange(BSTR* ActivePageName);
	};
	/////////////////////////////////////////////////////////////////////////////
	// Search wrapper class

	class Search : public COleDispatchDriver
	{
	public:
	Search() {}		// Calls COleDispatchDriver default constructor
	Search(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Search(const Search& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	CString GetFilter();
	BOOL GetIsSynchronous();
	LPDISPATCH GetResults();
	BOOL GetSearchSubFolders();
	void Stop();
	CString GetTag();
	CString GetScope();
	LPDISPATCH Save(LPCTSTR Name);
	LPDISPATCH GetTable();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _Results wrapper class

	class _Results : public COleDispatchDriver
	{
	public:
	_Results() {}		// Calls COleDispatchDriver default constructor
	_Results(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Results(const _Results& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH GetFirst();
	LPDISPATCH GetLast();
	LPDISPATCH GetNext();
	LPDISPATCH GetPrevious();
	void ResetColumns();
	void SetColumns(LPCTSTR Columns);
	void Sort(LPCTSTR Property_, const VARIANT& Descending);
	long GetDefaultItemType();
	void SetDefaultItemType(long nNewValue);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _Reminders wrapper class

	class _Reminders : public COleDispatchDriver
	{
	public:
	_Reminders() {}		// Calls COleDispatchDriver default constructor
	_Reminders(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Reminders(const _Reminders& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	void Remove(const VARIANT& Index);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _Reminder wrapper class

	class _Reminder : public COleDispatchDriver
	{
	public:
	_Reminder() {}		// Calls COleDispatchDriver default constructor
	_Reminder(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Reminder(const _Reminder& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	void Dismiss();
	void Snooze(const VARIANT& SnoozeTime);
	CString GetCaption();
	BOOL GetIsVisible();
	LPDISPATCH GetItem();
	DATE GetNextReminderDate();
	DATE GetOriginalReminderDate();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _TimeZones wrapper class

	class _TimeZones : public COleDispatchDriver
	{
	public:
	_TimeZones() {}		// Calls COleDispatchDriver default constructor
	_TimeZones(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_TimeZones(const _TimeZones& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	DATE ConvertTime(DATE SourceDateTime, LPDISPATCH SourceTimeZone, LPDISPATCH DestinationTimeZone);
	LPDISPATCH GetCurrentTimeZone();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _OlkTimeZoneControl wrapper class

	class _OlkTimeZoneControl : public COleDispatchDriver
	{
	public:
	_OlkTimeZoneControl() {}		// Calls COleDispatchDriver default constructor
	_OlkTimeZoneControl(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_OlkTimeZoneControl(const _OlkTimeZoneControl& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	long GetAppointmentTimeField();
	void SetAppointmentTimeField(long nNewValue);
	long GetBorderStyle();
	void SetBorderStyle(long nNewValue);
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	LPDISPATCH GetMouseIcon();
	void SetMouseIcon(LPDISPATCH newValue);
	long GetMousePointer();
	void SetMousePointer(long nNewValue);
	long GetSelectedTimeZoneIndex();
	void SetSelectedTimeZoneIndex(long nNewValue);
	VARIANT GetValue();
	void SetValue(const VARIANT& newValue);
	void DropDown();
	};
	/////////////////////////////////////////////////////////////////////////////
	// OlkTimeZoneControlEvents wrapper class

	class OlkTimeZoneControlEvents : public COleDispatchDriver
	{
	public:
	OlkTimeZoneControlEvents() {}		// Calls COleDispatchDriver default constructor
	OlkTimeZoneControlEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	OlkTimeZoneControlEvents(const OlkTimeZoneControlEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	void Click();
	void DoubleClick();
	void MouseDown(long Button, long Shift, float X, float Y);
	void MouseMove(long Button, long Shift, float X, float Y);
	void MouseUp(long Button, long Shift, float X, float Y);
	void Enter();
	void Exit(BOOL* Cancel);
	void KeyDown(long* KeyCode, long Shift);
	void KeyPress(long* KeyAscii);
	void KeyUp(long* KeyCode, long Shift);
	void Change();
	void DropButtonClick();
	void AfterUpdate();
	void BeforeUpdate(BOOL* Cancel);
	};
	/////////////////////////////////////////////////////////////////////////////
	// ApplicationEvents wrapper class

	class ApplicationEvents : public COleDispatchDriver
	{
	public:
	ApplicationEvents() {}		// Calls COleDispatchDriver default constructor
	ApplicationEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	ApplicationEvents(const ApplicationEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	void ItemSend(LPDISPATCH Item, BOOL* Cancel);
	void NewMail();
	void Reminder(LPDISPATCH Item);
	void OptionsPagesAdd(LPDISPATCH Pages);
	void Startup();
	void Quit();
	};
	/////////////////////////////////////////////////////////////////////////////
	// PropertyPages wrapper class

	class PropertyPages : public COleDispatchDriver
	{
	public:
	PropertyPages() {}		// Calls COleDispatchDriver default constructor
	PropertyPages(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	PropertyPages(const PropertyPages& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	void Add(const VARIANT& Page, LPCTSTR Title);
	void Remove(const VARIANT& Index);
	};
	/////////////////////////////////////////////////////////////////////////////
	// RecurrencePattern wrapper class

	class RecurrencePattern : public COleDispatchDriver
	{
	public:
	RecurrencePattern() {}		// Calls COleDispatchDriver default constructor
	RecurrencePattern(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	RecurrencePattern(const RecurrencePattern& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetDayOfMonth();
	void SetDayOfMonth(long nNewValue);
	long GetDayOfWeekMask();
	void SetDayOfWeekMask(long nNewValue);
	long GetDuration();
	void SetDuration(long nNewValue);
	DATE GetEndTime();
	void SetEndTime(DATE newValue);
	LPDISPATCH GetExceptions();
	long GetInstance();
	void SetInstance(long nNewValue);
	long GetInterval();
	void SetInterval(long nNewValue);
	long GetMonthOfYear();
	void SetMonthOfYear(long nNewValue);
	BOOL GetNoEndDate();
	void SetNoEndDate(BOOL bNewValue);
	long GetOccurrences();
	void SetOccurrences(long nNewValue);
	DATE GetPatternEndDate();
	void SetPatternEndDate(DATE newValue);
	DATE GetPatternStartDate();
	void SetPatternStartDate(DATE newValue);
	long GetRecurrenceType();
	void SetRecurrenceType(long nNewValue);
	BOOL GetRegenerate();
	void SetRegenerate(BOOL bNewValue);
	DATE GetStartTime();
	void SetStartTime(DATE newValue);
	LPDISPATCH GetOccurrence(DATE StartDate);
	};
	/////////////////////////////////////////////////////////////////////////////
	// Exceptions wrapper class

	class Exceptions : public COleDispatchDriver
	{
	public:
	Exceptions() {}		// Calls COleDispatchDriver default constructor
	Exceptions(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Exceptions(const Exceptions& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	};
	/////////////////////////////////////////////////////////////////////////////
	// Exception wrapper class

	class Exception : public COleDispatchDriver
	{
	public:
	Exception() {}		// Calls COleDispatchDriver default constructor
	Exception(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Exception(const Exception& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	LPDISPATCH GetAppointmentItem();
	BOOL GetDeleted();
	DATE GetOriginalDate();
	LPDISPATCH GetItemProperties();
	};
	*/

	/*
	/////////////////////////////////////////////////////////////////////////////
	// _MeetingItem wrapper class

	class _MeetingItem : public COleDispatchDriver
	{
	public:
	_MeetingItem() {}		// Calls COleDispatchDriver default constructor
	_MeetingItem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_MeetingItem(const _MeetingItem& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	LPDISPATCH GetActions();
	LPDISPATCH GetAttachments();
	CString GetBillingInformation();
	void SetBillingInformation(LPCTSTR lpszNewValue);
	CString GetBody();
	void SetBody(LPCTSTR lpszNewValue);
	CString GetCategories();
	void SetCategories(LPCTSTR lpszNewValue);
	CString GetCompanies();
	void SetCompanies(LPCTSTR lpszNewValue);
	CString GetConversationIndex();
	CString GetConversationTopic();
	DATE GetCreationTime();
	CString GetEntryID();
	LPDISPATCH GetFormDescription();
	LPDISPATCH GetGetInspector();
	long GetImportance();
	void SetImportance(long nNewValue);
	DATE GetLastModificationTime();
	CString GetMessageClass();
	void SetMessageClass(LPCTSTR lpszNewValue);
	CString GetMileage();
	void SetMileage(LPCTSTR lpszNewValue);
	BOOL GetNoAging();
	void SetNoAging(BOOL bNewValue);
	long GetOutlookInternalVersion();
	CString GetOutlookVersion();
	BOOL GetSaved();
	long GetSensitivity();
	void SetSensitivity(long nNewValue);
	long GetSize();
	CString GetSubject();
	void SetSubject(LPCTSTR lpszNewValue);
	BOOL GetUnRead();
	void SetUnRead(BOOL bNewValue);
	LPDISPATCH GetUserProperties();
	void Close(long SaveMode);
	LPDISPATCH Copy();
	void Delete();
	void Display(const VARIANT& Modal);
	LPDISPATCH Move(LPDISPATCH DestFldr);
	void PrintOut();
	void Save();
	void SaveAs(LPCTSTR Path, const VARIANT& Type);
	BOOL GetAutoForwarded();
	void SetAutoForwarded(BOOL bNewValue);
	DATE GetDeferredDeliveryTime();
	void SetDeferredDeliveryTime(DATE newValue);
	BOOL GetDeleteAfterSubmit();
	void SetDeleteAfterSubmit(BOOL bNewValue);
	DATE GetExpiryTime();
	void SetExpiryTime(DATE newValue);
	BOOL GetOriginatorDeliveryReportRequested();
	void SetOriginatorDeliveryReportRequested(BOOL bNewValue);
	DATE GetReceivedTime();
	void SetReceivedTime(DATE newValue);
	LPDISPATCH GetRecipients();
	BOOL GetReminderSet();
	void SetReminderSet(BOOL bNewValue);
	DATE GetReminderTime();
	void SetReminderTime(DATE newValue);
	LPDISPATCH GetReplyRecipients();
	LPDISPATCH GetSaveSentMessageFolder();
	void SetRefSaveSentMessageFolder(LPDISPATCH newValue);
	CString GetSenderName();
	BOOL GetSent();
	DATE GetSentOn();
	BOOL GetSubmitted();
	LPDISPATCH Forward();
	LPDISPATCH GetAssociatedAppointment(BOOL AddToCalendar);
	LPDISPATCH Reply();
	LPDISPATCH ReplyAll();
	void Send();
	LPDISPATCH GetLinks();
	long GetDownloadState();
	void ShowCategoriesDialog();
	LPDISPATCH GetItemProperties();
	long GetMarkForDownload();
	void SetMarkForDownload(long nNewValue);
	BOOL GetIsConflict();
	CString GetMeetingWorkspaceURL();
	BOOL GetAutoResolvedWinner();
	LPDISPATCH GetConflicts();
	CString GetSenderEmailAddress();
	CString GetSenderEmailType();
	LPDISPATCH GetPropertyAccessor();
	};
	/////////////////////////////////////////////////////////////////////////////
	// ExplorersEvents wrapper class

	class ExplorersEvents : public COleDispatchDriver
	{
	public:
	ExplorersEvents() {}		// Calls COleDispatchDriver default constructor
	ExplorersEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	ExplorersEvents(const ExplorersEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	void NewExplorer(LPDISPATCH Explorer);
	};
	/////////////////////////////////////////////////////////////////////////////
	// FoldersEvents wrapper class

	class FoldersEvents : public COleDispatchDriver
	{
	public:
	FoldersEvents() {}		// Calls COleDispatchDriver default constructor
	FoldersEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	FoldersEvents(const FoldersEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	void FolderAdd(LPDISPATCH Folder);
	void FolderChange(LPDISPATCH Folder);
	void FolderRemove();
	};
	/////////////////////////////////////////////////////////////////////////////
	// InspectorsEvents wrapper class

	class InspectorsEvents : public COleDispatchDriver
	{
	public:
	InspectorsEvents() {}		// Calls COleDispatchDriver default constructor
	InspectorsEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	InspectorsEvents(const InspectorsEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	void NewInspector(LPDISPATCH Inspector);
	};
	/////////////////////////////////////////////////////////////////////////////
	// ItemsEvents wrapper class

	class ItemsEvents : public COleDispatchDriver
	{
	public:
	ItemsEvents() {}		// Calls COleDispatchDriver default constructor
	ItemsEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	ItemsEvents(const ItemsEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	void ItemAdd(LPDISPATCH Item);
	void ItemChange(LPDISPATCH Item);
	void ItemRemove();
	};
	/////////////////////////////////////////////////////////////////////////////
	// NameSpaceEvents wrapper class

	class NameSpaceEvents : public COleDispatchDriver
	{
	public:
	NameSpaceEvents() {}		// Calls COleDispatchDriver default constructor
	NameSpaceEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	NameSpaceEvents(const NameSpaceEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	void OptionsPagesAdd(LPDISPATCH Pages, LPDISPATCH Folder);
	void AutoDiscoverComplete();
	};
	/////////////////////////////////////////////////////////////////////////////
	// OutlookBarGroup wrapper class

	class OutlookBarGroup : public COleDispatchDriver
	{
	public:
	OutlookBarGroup() {}		// Calls COleDispatchDriver default constructor
	OutlookBarGroup(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	OutlookBarGroup(const OutlookBarGroup& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	LPDISPATCH GetShortcuts();
	long GetViewType();
	void SetViewType(long nNewValue);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _OutlookBarShortcuts wrapper class

	class _OutlookBarShortcuts : public COleDispatchDriver
	{
	public:
	_OutlookBarShortcuts() {}		// Calls COleDispatchDriver default constructor
	_OutlookBarShortcuts(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_OutlookBarShortcuts(const _OutlookBarShortcuts& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH Add(const VARIANT& Target, LPCTSTR Name, const VARIANT& Index);
	void Remove(const VARIANT& Index);
	};
	/////////////////////////////////////////////////////////////////////////////
	// OutlookBarShortcut wrapper class

	class OutlookBarShortcut : public COleDispatchDriver
	{
	public:
	OutlookBarShortcut() {}		// Calls COleDispatchDriver default constructor
	OutlookBarShortcut(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	OutlookBarShortcut(const OutlookBarShortcut& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	VARIANT GetTarget();
	void SetIcon(const VARIANT& Icon);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _OutlookBarGroups wrapper class

	class _OutlookBarGroups : public COleDispatchDriver
	{
	public:
	_OutlookBarGroups() {}		// Calls COleDispatchDriver default constructor
	_OutlookBarGroups(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_OutlookBarGroups(const _OutlookBarGroups& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH Add(LPCTSTR Name, const VARIANT& Index);
	void Remove(const VARIANT& Index);
	};
	/////////////////////////////////////////////////////////////////////////////
	// OutlookBarGroupsEvents wrapper class

	class OutlookBarGroupsEvents : public COleDispatchDriver
	{
	public:
	OutlookBarGroupsEvents() {}		// Calls COleDispatchDriver default constructor
	OutlookBarGroupsEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	OutlookBarGroupsEvents(const OutlookBarGroupsEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	void GroupAdd(LPDISPATCH NewGroup);
	void BeforeGroupAdd(BOOL* Cancel);
	void BeforeGroupRemove(LPDISPATCH Group, BOOL* Cancel);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _OutlookBarPane wrapper class

	class _OutlookBarPane : public COleDispatchDriver
	{
	public:
	_OutlookBarPane() {}		// Calls COleDispatchDriver default constructor
	_OutlookBarPane(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_OutlookBarPane(const _OutlookBarPane& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	LPDISPATCH GetContents();
	CString GetName();
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	};
	/////////////////////////////////////////////////////////////////////////////
	// OutlookBarStorage wrapper class

	class OutlookBarStorage : public COleDispatchDriver
	{
	public:
	OutlookBarStorage() {}		// Calls COleDispatchDriver default constructor
	OutlookBarStorage(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	OutlookBarStorage(const OutlookBarStorage& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	LPDISPATCH GetGroups();
	};
	/////////////////////////////////////////////////////////////////////////////
	// OutlookBarPaneEvents wrapper class

	class OutlookBarPaneEvents : public COleDispatchDriver
	{
	public:
	OutlookBarPaneEvents() {}		// Calls COleDispatchDriver default constructor
	OutlookBarPaneEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	OutlookBarPaneEvents(const OutlookBarPaneEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	void BeforeNavigate(LPDISPATCH Shortcut, BOOL* Cancel);
	};
	/////////////////////////////////////////////////////////////////////////////
	// OutlookBarShortcutsEvents wrapper class

	class OutlookBarShortcutsEvents : public COleDispatchDriver
	{
	public:
	OutlookBarShortcutsEvents() {}		// Calls COleDispatchDriver default constructor
	OutlookBarShortcutsEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	OutlookBarShortcutsEvents(const OutlookBarShortcutsEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	void ShortcutAdd(LPDISPATCH NewShortcut);
	void BeforeShortcutAdd(BOOL* Cancel);
	void BeforeShortcutRemove(LPDISPATCH Shortcut, BOOL* Cancel);
	};
	/////////////////////////////////////////////////////////////////////////////
	// PropertyPageSite wrapper class

	class PropertyPageSite : public COleDispatchDriver
	{
	public:
	PropertyPageSite() {}		// Calls COleDispatchDriver default constructor
	PropertyPageSite(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	PropertyPageSite(const PropertyPageSite& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	void OnStatusChange();
	};
	/////////////////////////////////////////////////////////////////////////////
	// Pages wrapper class

	class Pages : public COleDispatchDriver
	{
	public:
	Pages() {}		// Calls COleDispatchDriver default constructor
	Pages(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Pages(const Pages& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH Add(const VARIANT& Name);
	void Remove(long Index);
	};
	/////////////////////////////////////////////////////////////////////////////
	// ApplicationEvents_10 wrapper class

	class ApplicationEvents_10 : public COleDispatchDriver
	{
	public:
	ApplicationEvents_10() {}		// Calls COleDispatchDriver default constructor
	ApplicationEvents_10(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	ApplicationEvents_10(const ApplicationEvents_10& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	// method 'ItemSend' not emitted because of invalid return type or parameter type
	// method 'NewMail' not emitted because of invalid return type or parameter type
	// method 'Reminder' not emitted because of invalid return type or parameter type
	// method 'OptionsPagesAdd' not emitted because of invalid return type or parameter type
	// method 'Startup' not emitted because of invalid return type or parameter type
	// method 'Quit' not emitted because of invalid return type or parameter type
	void AdvancedSearchComplete(LPDISPATCH SearchObject);
	void AdvancedSearchStopped(LPDISPATCH SearchObject);
	void MAPILogonComplete();
	};
	/////////////////////////////////////////////////////////////////////////////
	// ApplicationEvents_11 wrapper class

	class ApplicationEvents_11 : public COleDispatchDriver
	{
	public:
	ApplicationEvents_11() {}		// Calls COleDispatchDriver default constructor
	ApplicationEvents_11(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	ApplicationEvents_11(const ApplicationEvents_11& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	// method 'ItemSend' not emitted because of invalid return type or parameter type
	// method 'NewMail' not emitted because of invalid return type or parameter type
	// method 'Reminder' not emitted because of invalid return type or parameter type
	// method 'OptionsPagesAdd' not emitted because of invalid return type or parameter type
	// method 'Startup' not emitted because of invalid return type or parameter type
	// method 'Quit' not emitted because of invalid return type or parameter type
	// method 'AdvancedSearchComplete' not emitted because of invalid return type or parameter type
	// method 'AdvancedSearchStopped' not emitted because of invalid return type or parameter type
	// method 'MAPILogonComplete' not emitted because of invalid return type or parameter type
	void NewMailEx(LPCTSTR EntryIDCollection);
	// method 'AttachmentContextMenuDisplay' not emitted because of invalid return type or parameter type
	void FolderContextMenuDisplay(LPDISPATCH CommandBar, LPDISPATCH Folder);
	void StoreContextMenuDisplay(LPDISPATCH CommandBar, LPDISPATCH Store);
	void ShortcutContextMenuDisplay(LPDISPATCH CommandBar, LPDISPATCH Shortcut);
	void ViewContextMenuDisplay(LPDISPATCH CommandBar, LPDISPATCH View);
	void ItemContextMenuDisplay(LPDISPATCH CommandBar, LPDISPATCH Selection);
	void ContextMenuClose(long ContextMenu);
	void ItemLoad(LPDISPATCH Item);
	void BeforeFolderSharingDialog(LPDISPATCH FolderToShare, BOOL* Cancel);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _AttachmentSelection wrapper class

	class _AttachmentSelection : public COleDispatchDriver
	{
	public:
	_AttachmentSelection() {}		// Calls COleDispatchDriver default constructor
	_AttachmentSelection(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_AttachmentSelection(const _AttachmentSelection& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	};
	/////////////////////////////////////////////////////////////////////////////
	// MAPIFolderEvents_12 wrapper class

	class MAPIFolderEvents_12 : public COleDispatchDriver
	{
	public:
	MAPIFolderEvents_12() {}		// Calls COleDispatchDriver default constructor
	MAPIFolderEvents_12(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	MAPIFolderEvents_12(const MAPIFolderEvents_12& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	void BeforeFolderMove(LPDISPATCH MoveTo, BOOL* Cancel);
	void BeforeItemMove(LPDISPATCH Item, LPDISPATCH MoveTo, BOOL* Cancel);
	};
	/////////////////////////////////////////////////////////////////////////////
	// ResultsEvents wrapper class

	class ResultsEvents : public COleDispatchDriver
	{
	public:
	ResultsEvents() {}		// Calls COleDispatchDriver default constructor
	ResultsEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	ResultsEvents(const ResultsEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	void ItemAdd(LPDISPATCH Item);
	void ItemChange(LPDISPATCH Item);
	void ItemRemove();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _ViewsEvents wrapper class

	class _ViewsEvents : public COleDispatchDriver
	{
	public:
	_ViewsEvents() {}		// Calls COleDispatchDriver default constructor
	_ViewsEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_ViewsEvents(const _ViewsEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	void ViewAdd(LPDISPATCH View);
	void ViewRemove(LPDISPATCH View);
	};
	/////////////////////////////////////////////////////////////////////////////
	// ReminderCollectionEvents wrapper class

	class ReminderCollectionEvents : public COleDispatchDriver
	{
	public:
	ReminderCollectionEvents() {}		// Calls COleDispatchDriver default constructor
	ReminderCollectionEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	ReminderCollectionEvents(const ReminderCollectionEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	void BeforeReminderShow(BOOL* Cancel);
	void ReminderAdd(LPDISPATCH ReminderObject);
	void ReminderChange(LPDISPATCH ReminderObject);
	void ReminderFire(LPDISPATCH ReminderObject);
	void ReminderRemove();
	void Snooze(LPDISPATCH ReminderObject);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _DistListItem wrapper class

	class _DistListItem : public COleDispatchDriver
	{
	public:
	_DistListItem() {}		// Calls COleDispatchDriver default constructor
	_DistListItem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_DistListItem(const _DistListItem& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	LPDISPATCH GetActions();
	LPDISPATCH GetAttachments();
	CString GetBillingInformation();
	void SetBillingInformation(LPCTSTR lpszNewValue);
	CString GetBody();
	void SetBody(LPCTSTR lpszNewValue);
	CString GetCategories();
	void SetCategories(LPCTSTR lpszNewValue);
	CString GetCompanies();
	void SetCompanies(LPCTSTR lpszNewValue);
	CString GetConversationIndex();
	CString GetConversationTopic();
	DATE GetCreationTime();
	CString GetEntryID();
	LPDISPATCH GetFormDescription();
	LPDISPATCH GetGetInspector();
	long GetImportance();
	void SetImportance(long nNewValue);
	DATE GetLastModificationTime();
	CString GetMessageClass();
	void SetMessageClass(LPCTSTR lpszNewValue);
	CString GetMileage();
	void SetMileage(LPCTSTR lpszNewValue);
	BOOL GetNoAging();
	void SetNoAging(BOOL bNewValue);
	long GetOutlookInternalVersion();
	CString GetOutlookVersion();
	BOOL GetSaved();
	long GetSensitivity();
	void SetSensitivity(long nNewValue);
	long GetSize();
	CString GetSubject();
	void SetSubject(LPCTSTR lpszNewValue);
	BOOL GetUnRead();
	void SetUnRead(BOOL bNewValue);
	LPDISPATCH GetUserProperties();
	void Close(long SaveMode);
	LPDISPATCH Copy();
	void Delete();
	void Display(const VARIANT& Modal);
	LPDISPATCH Move(LPDISPATCH DestFldr);
	void PrintOut();
	void Save();
	void SaveAs(LPCTSTR Path, const VARIANT& Type);
	CString GetDLName();
	void SetDLName(LPCTSTR lpszNewValue);
	long GetMemberCount();
	LPDISPATCH GetLinks();
	void AddMembers(LPDISPATCH Recipients);
	void RemoveMembers(LPDISPATCH Recipients);
	LPDISPATCH GetMember(long Index);
	long GetDownloadState();
	void ShowCategoriesDialog();
	void AddMember(LPDISPATCH Recipient);
	void RemoveMember(LPDISPATCH Recipient);
	LPDISPATCH GetItemProperties();
	long GetMarkForDownload();
	void SetMarkForDownload(long nNewValue);
	BOOL GetIsConflict();
	BOOL GetAutoResolvedWinner();
	LPDISPATCH GetConflicts();
	LPDISPATCH GetPropertyAccessor();
	CString GetTaskSubject();
	void SetTaskSubject(LPCTSTR lpszNewValue);
	DATE GetTaskDueDate();
	void SetTaskDueDate(DATE newValue);
	DATE GetTaskStartDate();
	void SetTaskStartDate(DATE newValue);
	DATE GetTaskCompletedDate();
	void SetTaskCompletedDate(DATE newValue);
	DATE GetToDoTaskOrdinal();
	void SetToDoTaskOrdinal(DATE newValue);
	BOOL GetReminderOverrideDefault();
	void SetReminderOverrideDefault(BOOL bNewValue);
	BOOL GetReminderPlaySound();
	void SetReminderPlaySound(BOOL bNewValue);
	BOOL GetReminderSet();
	void SetReminderSet(BOOL bNewValue);
	CString GetReminderSoundFile();
	void SetReminderSoundFile(LPCTSTR lpszNewValue);
	DATE GetReminderTime();
	void SetReminderTime(DATE newValue);
	void MarkAsTask(long MarkInterval);
	void ClearTaskFlag();
	BOOL GetIsMarkedAsTask();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _DocumentItem wrapper class

	class _DocumentItem : public COleDispatchDriver
	{
	public:
	_DocumentItem() {}		// Calls COleDispatchDriver default constructor
	_DocumentItem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_DocumentItem(const _DocumentItem& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	LPDISPATCH GetActions();
	LPDISPATCH GetAttachments();
	CString GetBillingInformation();
	void SetBillingInformation(LPCTSTR lpszNewValue);
	CString GetBody();
	void SetBody(LPCTSTR lpszNewValue);
	CString GetCategories();
	void SetCategories(LPCTSTR lpszNewValue);
	CString GetCompanies();
	void SetCompanies(LPCTSTR lpszNewValue);
	CString GetConversationIndex();
	CString GetConversationTopic();
	DATE GetCreationTime();
	CString GetEntryID();
	LPDISPATCH GetFormDescription();
	LPDISPATCH GetGetInspector();
	long GetImportance();
	void SetImportance(long nNewValue);
	DATE GetLastModificationTime();
	CString GetMessageClass();
	void SetMessageClass(LPCTSTR lpszNewValue);
	CString GetMileage();
	void SetMileage(LPCTSTR lpszNewValue);
	BOOL GetNoAging();
	void SetNoAging(BOOL bNewValue);
	long GetOutlookInternalVersion();
	CString GetOutlookVersion();
	BOOL GetSaved();
	long GetSensitivity();
	void SetSensitivity(long nNewValue);
	long GetSize();
	CString GetSubject();
	void SetSubject(LPCTSTR lpszNewValue);
	BOOL GetUnRead();
	void SetUnRead(BOOL bNewValue);
	LPDISPATCH GetUserProperties();
	void Close(long SaveMode);
	LPDISPATCH Copy();
	void Delete();
	void Display(const VARIANT& Modal);
	LPDISPATCH Move(LPDISPATCH DestFldr);
	void PrintOut();
	void Save();
	void SaveAs(LPCTSTR Path, const VARIANT& Type);
	LPDISPATCH GetLinks();
	long GetDownloadState();
	void ShowCategoriesDialog();
	LPDISPATCH GetItemProperties();
	long GetMarkForDownload();
	void SetMarkForDownload(long nNewValue);
	BOOL GetIsConflict();
	BOOL GetAutoResolvedWinner();
	LPDISPATCH GetConflicts();
	LPDISPATCH GetPropertyAccessor();
	};
	*/

	/*
	/////////////////////////////////////////////////////////////////////////////
	// _PostItem wrapper class

	class _PostItem : public COleDispatchDriver
	{
	public:
	_PostItem() {}		// Calls COleDispatchDriver default constructor
	_PostItem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_PostItem(const _PostItem& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	LPDISPATCH GetActions();
	LPDISPATCH GetAttachments();
	CString GetBillingInformation();
	void SetBillingInformation(LPCTSTR lpszNewValue);
	CString GetBody();
	void SetBody(LPCTSTR lpszNewValue);
	CString GetCategories();
	void SetCategories(LPCTSTR lpszNewValue);
	CString GetCompanies();
	void SetCompanies(LPCTSTR lpszNewValue);
	CString GetConversationIndex();
	CString GetConversationTopic();
	DATE GetCreationTime();
	CString GetEntryID();
	LPDISPATCH GetFormDescription();
	LPDISPATCH GetGetInspector();
	long GetImportance();
	void SetImportance(long nNewValue);
	DATE GetLastModificationTime();
	CString GetMessageClass();
	void SetMessageClass(LPCTSTR lpszNewValue);
	CString GetMileage();
	void SetMileage(LPCTSTR lpszNewValue);
	BOOL GetNoAging();
	void SetNoAging(BOOL bNewValue);
	long GetOutlookInternalVersion();
	CString GetOutlookVersion();
	BOOL GetSaved();
	long GetSensitivity();
	void SetSensitivity(long nNewValue);
	long GetSize();
	CString GetSubject();
	void SetSubject(LPCTSTR lpszNewValue);
	BOOL GetUnRead();
	void SetUnRead(BOOL bNewValue);
	LPDISPATCH GetUserProperties();
	void Close(long SaveMode);
	LPDISPATCH Copy();
	void Delete();
	void Display(const VARIANT& Modal);
	LPDISPATCH Move(LPDISPATCH DestFldr);
	void PrintOut();
	void Save();
	void SaveAs(LPCTSTR Path, const VARIANT& Type);
	DATE GetExpiryTime();
	void SetExpiryTime(DATE newValue);
	CString GetHTMLBody();
	void SetHTMLBody(LPCTSTR lpszNewValue);
	DATE GetReceivedTime();
	CString GetSenderName();
	DATE GetSentOn();
	void ClearConversationIndex();
	LPDISPATCH Forward();
	void Post();
	LPDISPATCH Reply();
	LPDISPATCH GetLinks();
	LPDISPATCH GetItemProperties();
	long GetBodyFormat();
	void SetBodyFormat(long nNewValue);
	long GetDownloadState();
	void ShowCategoriesDialog();
	long GetInternetCodepage();
	void SetInternetCodepage(long nNewValue);
	long GetMarkForDownload();
	void SetMarkForDownload(long nNewValue);
	BOOL GetIsConflict();
	BOOL GetAutoResolvedWinner();
	LPDISPATCH GetConflicts();
	CString GetSenderEmailAddress();
	CString GetSenderEmailType();
	LPDISPATCH GetPropertyAccessor();
	CString GetTaskSubject();
	void SetTaskSubject(LPCTSTR lpszNewValue);
	DATE GetTaskDueDate();
	void SetTaskDueDate(DATE newValue);
	DATE GetTaskStartDate();
	void SetTaskStartDate(DATE newValue);
	DATE GetTaskCompletedDate();
	void SetTaskCompletedDate(DATE newValue);
	DATE GetToDoTaskOrdinal();
	void SetToDoTaskOrdinal(DATE newValue);
	BOOL GetReminderOverrideDefault();
	void SetReminderOverrideDefault(BOOL bNewValue);
	BOOL GetReminderPlaySound();
	void SetReminderPlaySound(BOOL bNewValue);
	BOOL GetReminderSet();
	void SetReminderSet(BOOL bNewValue);
	CString GetReminderSoundFile();
	void SetReminderSoundFile(LPCTSTR lpszNewValue);
	DATE GetReminderTime();
	void SetReminderTime(DATE newValue);
	void MarkAsTask(long MarkInterval);
	void ClearTaskFlag();
	BOOL GetIsMarkedAsTask();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _RemoteItem wrapper class

	class _RemoteItem : public COleDispatchDriver
	{
	public:
	_RemoteItem() {}		// Calls COleDispatchDriver default constructor
	_RemoteItem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_RemoteItem(const _RemoteItem& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	LPDISPATCH GetActions();
	LPDISPATCH GetAttachments();
	CString GetBillingInformation();
	void SetBillingInformation(LPCTSTR lpszNewValue);
	CString GetBody();
	void SetBody(LPCTSTR lpszNewValue);
	CString GetCategories();
	void SetCategories(LPCTSTR lpszNewValue);
	CString GetCompanies();
	void SetCompanies(LPCTSTR lpszNewValue);
	CString GetConversationIndex();
	CString GetConversationTopic();
	DATE GetCreationTime();
	CString GetEntryID();
	LPDISPATCH GetFormDescription();
	LPDISPATCH GetGetInspector();
	long GetImportance();
	void SetImportance(long nNewValue);
	DATE GetLastModificationTime();
	CString GetMessageClass();
	void SetMessageClass(LPCTSTR lpszNewValue);
	CString GetMileage();
	void SetMileage(LPCTSTR lpszNewValue);
	BOOL GetNoAging();
	void SetNoAging(BOOL bNewValue);
	long GetOutlookInternalVersion();
	CString GetOutlookVersion();
	BOOL GetSaved();
	long GetSensitivity();
	void SetSensitivity(long nNewValue);
	long GetSize();
	CString GetSubject();
	void SetSubject(LPCTSTR lpszNewValue);
	BOOL GetUnRead();
	void SetUnRead(BOOL bNewValue);
	LPDISPATCH GetUserProperties();
	void Close(long SaveMode);
	LPDISPATCH Copy();
	void Delete();
	void Display(const VARIANT& Modal);
	LPDISPATCH Move(LPDISPATCH DestFldr);
	void PrintOut();
	void Save();
	void SaveAs(LPCTSTR Path, const VARIANT& Type);
	BOOL GetHasAttachment();
	CString GetRemoteMessageClass();
	long GetTransferSize();
	long GetTransferTime();
	LPDISPATCH GetLinks();
	long GetDownloadState();
	void ShowCategoriesDialog();
	LPDISPATCH GetItemProperties();
	long GetMarkForDownload();
	void SetMarkForDownload(long nNewValue);
	BOOL GetIsConflict();
	BOOL GetAutoResolvedWinner();
	LPDISPATCH GetConflicts();
	LPDISPATCH GetPropertyAccessor();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _ReportItem wrapper class

	class _ReportItem : public COleDispatchDriver
	{
	public:
	_ReportItem() {}		// Calls COleDispatchDriver default constructor
	_ReportItem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_ReportItem(const _ReportItem& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	LPDISPATCH GetActions();
	LPDISPATCH GetAttachments();
	CString GetBillingInformation();
	void SetBillingInformation(LPCTSTR lpszNewValue);
	CString GetBody();
	void SetBody(LPCTSTR lpszNewValue);
	CString GetCategories();
	void SetCategories(LPCTSTR lpszNewValue);
	CString GetCompanies();
	void SetCompanies(LPCTSTR lpszNewValue);
	CString GetConversationIndex();
	CString GetConversationTopic();
	DATE GetCreationTime();
	CString GetEntryID();
	LPDISPATCH GetFormDescription();
	LPDISPATCH GetGetInspector();
	long GetImportance();
	void SetImportance(long nNewValue);
	DATE GetLastModificationTime();
	CString GetMessageClass();
	void SetMessageClass(LPCTSTR lpszNewValue);
	CString GetMileage();
	void SetMileage(LPCTSTR lpszNewValue);
	BOOL GetNoAging();
	void SetNoAging(BOOL bNewValue);
	long GetOutlookInternalVersion();
	CString GetOutlookVersion();
	BOOL GetSaved();
	long GetSensitivity();
	void SetSensitivity(long nNewValue);
	long GetSize();
	CString GetSubject();
	void SetSubject(LPCTSTR lpszNewValue);
	BOOL GetUnRead();
	void SetUnRead(BOOL bNewValue);
	LPDISPATCH GetUserProperties();
	void Close(long SaveMode);
	LPDISPATCH Copy();
	void Delete();
	void Display(const VARIANT& Modal);
	LPDISPATCH Move(LPDISPATCH DestFldr);
	void PrintOut();
	void Save();
	void SaveAs(LPCTSTR Path, const VARIANT& Type);
	LPDISPATCH GetLinks();
	long GetDownloadState();
	void ShowCategoriesDialog();
	LPDISPATCH GetItemProperties();
	long GetMarkForDownload();
	void SetMarkForDownload(long nNewValue);
	BOOL GetIsConflict();
	BOOL GetAutoResolvedWinner();
	LPDISPATCH GetConflicts();
	LPDISPATCH GetPropertyAccessor();
	};
	*/


	/////////////////////////////////////////////////////////////////////////////
	// _TaskRequestAcceptItem wrapper class
	/*
	class _TaskRequestAcceptItem : public COleDispatchDriver
	{
	public:
	_TaskRequestAcceptItem() {}		// Calls COleDispatchDriver default constructor
	_TaskRequestAcceptItem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_TaskRequestAcceptItem(const _TaskRequestAcceptItem& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	LPDISPATCH GetActions();
	LPDISPATCH GetAttachments();
	CString GetBillingInformation();
	void SetBillingInformation(LPCTSTR lpszNewValue);
	CString GetBody();
	void SetBody(LPCTSTR lpszNewValue);
	CString GetCategories();
	void SetCategories(LPCTSTR lpszNewValue);
	CString GetCompanies();
	void SetCompanies(LPCTSTR lpszNewValue);
	CString GetConversationIndex();
	CString GetConversationTopic();
	DATE GetCreationTime();
	CString GetEntryID();
	LPDISPATCH GetFormDescription();
	LPDISPATCH GetGetInspector();
	long GetImportance();
	void SetImportance(long nNewValue);
	DATE GetLastModificationTime();
	CString GetMessageClass();
	void SetMessageClass(LPCTSTR lpszNewValue);
	CString GetMileage();
	void SetMileage(LPCTSTR lpszNewValue);
	BOOL GetNoAging();
	void SetNoAging(BOOL bNewValue);
	long GetOutlookInternalVersion();
	CString GetOutlookVersion();
	BOOL GetSaved();
	long GetSensitivity();
	void SetSensitivity(long nNewValue);
	long GetSize();
	CString GetSubject();
	void SetSubject(LPCTSTR lpszNewValue);
	BOOL GetUnRead();
	void SetUnRead(BOOL bNewValue);
	LPDISPATCH GetUserProperties();
	void Close(long SaveMode);
	LPDISPATCH Copy();
	void Delete();
	void Display(const VARIANT& Modal);
	LPDISPATCH Move(LPDISPATCH DestFldr);
	void PrintOut();
	void Save();
	void SaveAs(LPCTSTR Path, const VARIANT& Type);
	LPDISPATCH GetAssociatedTask(BOOL AddToTaskList);
	LPDISPATCH GetLinks();
	long GetDownloadState();
	void ShowCategoriesDialog();
	LPDISPATCH GetItemProperties();
	long GetMarkForDownload();
	void SetMarkForDownload(long nNewValue);
	BOOL GetIsConflict();
	BOOL GetAutoResolvedWinner();
	LPDISPATCH GetConflicts();
	LPDISPATCH GetPropertyAccessor();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _TaskRequestDeclineItem wrapper class

	class _TaskRequestDeclineItem : public COleDispatchDriver
	{
	public:
	_TaskRequestDeclineItem() {}		// Calls COleDispatchDriver default constructor
	_TaskRequestDeclineItem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_TaskRequestDeclineItem(const _TaskRequestDeclineItem& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	LPDISPATCH GetActions();
	LPDISPATCH GetAttachments();
	CString GetBillingInformation();
	void SetBillingInformation(LPCTSTR lpszNewValue);
	CString GetBody();
	void SetBody(LPCTSTR lpszNewValue);
	CString GetCategories();
	void SetCategories(LPCTSTR lpszNewValue);
	CString GetCompanies();
	void SetCompanies(LPCTSTR lpszNewValue);
	CString GetConversationIndex();
	CString GetConversationTopic();
	DATE GetCreationTime();
	CString GetEntryID();
	LPDISPATCH GetFormDescription();
	LPDISPATCH GetGetInspector();
	long GetImportance();
	void SetImportance(long nNewValue);
	DATE GetLastModificationTime();
	CString GetMessageClass();
	void SetMessageClass(LPCTSTR lpszNewValue);
	CString GetMileage();
	void SetMileage(LPCTSTR lpszNewValue);
	BOOL GetNoAging();
	void SetNoAging(BOOL bNewValue);
	long GetOutlookInternalVersion();
	CString GetOutlookVersion();
	BOOL GetSaved();
	long GetSensitivity();
	void SetSensitivity(long nNewValue);
	long GetSize();
	CString GetSubject();
	void SetSubject(LPCTSTR lpszNewValue);
	BOOL GetUnRead();
	void SetUnRead(BOOL bNewValue);
	LPDISPATCH GetUserProperties();
	void Close(long SaveMode);
	LPDISPATCH Copy();
	void Delete();
	void Display(const VARIANT& Modal);
	LPDISPATCH Move(LPDISPATCH DestFldr);
	void PrintOut();
	void Save();
	void SaveAs(LPCTSTR Path, const VARIANT& Type);
	LPDISPATCH GetAssociatedTask(BOOL AddToTaskList);
	LPDISPATCH GetLinks();
	long GetDownloadState();
	void ShowCategoriesDialog();
	LPDISPATCH GetItemProperties();
	long GetMarkForDownload();
	void SetMarkForDownload(long nNewValue);
	BOOL GetIsConflict();
	BOOL GetAutoResolvedWinner();
	LPDISPATCH GetConflicts();
	LPDISPATCH GetPropertyAccessor();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _TaskRequestItem wrapper class

	class _TaskRequestItem : public COleDispatchDriver
	{
	public:
	_TaskRequestItem() {}		// Calls COleDispatchDriver default constructor
	_TaskRequestItem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_TaskRequestItem(const _TaskRequestItem& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	LPDISPATCH GetActions();
	LPDISPATCH GetAttachments();
	CString GetBillingInformation();
	void SetBillingInformation(LPCTSTR lpszNewValue);
	CString GetBody();
	void SetBody(LPCTSTR lpszNewValue);
	CString GetCategories();
	void SetCategories(LPCTSTR lpszNewValue);
	CString GetCompanies();
	void SetCompanies(LPCTSTR lpszNewValue);
	CString GetConversationIndex();
	CString GetConversationTopic();
	DATE GetCreationTime();
	CString GetEntryID();
	LPDISPATCH GetFormDescription();
	LPDISPATCH GetGetInspector();
	long GetImportance();
	void SetImportance(long nNewValue);
	DATE GetLastModificationTime();
	CString GetMessageClass();
	void SetMessageClass(LPCTSTR lpszNewValue);
	CString GetMileage();
	void SetMileage(LPCTSTR lpszNewValue);
	BOOL GetNoAging();
	void SetNoAging(BOOL bNewValue);
	long GetOutlookInternalVersion();
	CString GetOutlookVersion();
	BOOL GetSaved();
	long GetSensitivity();
	void SetSensitivity(long nNewValue);
	long GetSize();
	CString GetSubject();
	void SetSubject(LPCTSTR lpszNewValue);
	BOOL GetUnRead();
	void SetUnRead(BOOL bNewValue);
	LPDISPATCH GetUserProperties();
	void Close(long SaveMode);
	LPDISPATCH Copy();
	void Delete();
	void Display(const VARIANT& Modal);
	LPDISPATCH Move(LPDISPATCH DestFldr);
	void PrintOut();
	void Save();
	void SaveAs(LPCTSTR Path, const VARIANT& Type);
	LPDISPATCH GetAssociatedTask(BOOL AddToTaskList);
	LPDISPATCH GetLinks();
	long GetDownloadState();
	void ShowCategoriesDialog();
	LPDISPATCH GetItemProperties();
	long GetMarkForDownload();
	void SetMarkForDownload(long nNewValue);
	BOOL GetIsConflict();
	BOOL GetAutoResolvedWinner();
	LPDISPATCH GetConflicts();
	LPDISPATCH GetPropertyAccessor();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _TaskRequestUpdateItem wrapper class

	class _TaskRequestUpdateItem : public COleDispatchDriver
	{
	public:
	_TaskRequestUpdateItem() {}		// Calls COleDispatchDriver default constructor
	_TaskRequestUpdateItem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_TaskRequestUpdateItem(const _TaskRequestUpdateItem& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	LPDISPATCH GetActions();
	LPDISPATCH GetAttachments();
	CString GetBillingInformation();
	void SetBillingInformation(LPCTSTR lpszNewValue);
	CString GetBody();
	void SetBody(LPCTSTR lpszNewValue);
	CString GetCategories();
	void SetCategories(LPCTSTR lpszNewValue);
	CString GetCompanies();
	void SetCompanies(LPCTSTR lpszNewValue);
	CString GetConversationIndex();
	CString GetConversationTopic();
	DATE GetCreationTime();
	CString GetEntryID();
	LPDISPATCH GetFormDescription();
	LPDISPATCH GetGetInspector();
	long GetImportance();
	void SetImportance(long nNewValue);
	DATE GetLastModificationTime();
	CString GetMessageClass();
	void SetMessageClass(LPCTSTR lpszNewValue);
	CString GetMileage();
	void SetMileage(LPCTSTR lpszNewValue);
	BOOL GetNoAging();
	void SetNoAging(BOOL bNewValue);
	long GetOutlookInternalVersion();
	CString GetOutlookVersion();
	BOOL GetSaved();
	long GetSensitivity();
	void SetSensitivity(long nNewValue);
	long GetSize();
	CString GetSubject();
	void SetSubject(LPCTSTR lpszNewValue);
	BOOL GetUnRead();
	void SetUnRead(BOOL bNewValue);
	LPDISPATCH GetUserProperties();
	void Close(long SaveMode);
	LPDISPATCH Copy();
	void Delete();
	void Display(const VARIANT& Modal);
	LPDISPATCH Move(LPDISPATCH DestFldr);
	void PrintOut();
	void Save();
	void SaveAs(LPCTSTR Path, const VARIANT& Type);
	LPDISPATCH GetAssociatedTask(BOOL AddToTaskList);
	LPDISPATCH GetLinks();
	long GetDownloadState();
	void ShowCategoriesDialog();
	LPDISPATCH GetItemProperties();
	long GetMarkForDownload();
	void SetMarkForDownload(long nNewValue);
	BOOL GetIsConflict();
	BOOL GetAutoResolvedWinner();
	LPDISPATCH GetConflicts();
	LPDISPATCH GetPropertyAccessor();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _FormRegion wrapper class

	class _FormRegion : public COleDispatchDriver
	{
	public:
	_FormRegion() {}		// Calls COleDispatchDriver default constructor
	_FormRegion(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_FormRegion(const _FormRegion& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	BOOL GetIsExpanded();
	LPDISPATCH GetForm();
	CString GetDetail();
	void SetDetail(LPCTSTR lpszNewValue);
	LPDISPATCH GetItem();
	LPDISPATCH GetInspector();
	long GetFormRegionMode();
	long GetLanguage();
	void Reflow();
	CString GetDisplayName();
	CString GetInternalName();
	void SetControlItemProperty(LPDISPATCH Control, LPCTSTR PropertyName);
	BOOL GetSuppressControlReplacement();
	void SetSuppressControlReplacement(BOOL bNewValue);
	void Select();
	BOOL GetEnableAutoLayout();
	void SetEnableAutoLayout(BOOL bNewValue);
	};
	/////////////////////////////////////////////////////////////////////////////
	// FormRegionEvents wrapper class

	class FormRegionEvents : public COleDispatchDriver
	{
	public:
	FormRegionEvents() {}		// Calls COleDispatchDriver default constructor
	FormRegionEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	FormRegionEvents(const FormRegionEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	void Expanded(BOOL Expand);
	void Close();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _TableView wrapper class

	class _TableView : public COleDispatchDriver
	{
	public:
	_TableView() {}		// Calls COleDispatchDriver default constructor
	_TableView(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_TableView(const _TableView& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	void Apply();
	LPDISPATCH Copy(LPCTSTR Name, long SaveOption);
	void Delete();
	void Reset();
	void Save();
	CString GetLanguage();
	void SetLanguage(LPCTSTR lpszNewValue);
	BOOL GetLockUserChanges();
	void SetLockUserChanges(BOOL bNewValue);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	long GetSaveOption();
	BOOL GetStandard();
	long GetViewType();
	CString GetXml();
	void SetXml(LPCTSTR lpszNewValue);
	void GoToDate(DATE Date);
	CString GetFilter();
	void SetFilter(LPCTSTR lpszNewValue);
	LPDISPATCH GetViewFields();
	LPDISPATCH GetGroupByFields();
	LPDISPATCH GetSortFields();
	long GetMaxLinesInMultiLineView();
	void SetMaxLinesInMultiLineView(long nNewValue);
	BOOL GetAutomaticGrouping();
	void SetAutomaticGrouping(BOOL bNewValue);
	long GetDefaultExpandCollapseSetting();
	void SetDefaultExpandCollapseSetting(long nNewValue);
	BOOL GetAutomaticColumnSizing();
	void SetAutomaticColumnSizing(BOOL bNewValue);
	long GetMultiLine();
	void SetMultiLine(long nNewValue);
	long GetMultiLineWidth();
	void SetMultiLineWidth(long nNewValue);
	BOOL GetAllowInCellEditing();
	void SetAllowInCellEditing(BOOL bNewValue);
	BOOL GetShowNewItemRow();
	void SetShowNewItemRow(BOOL bNewValue);
	long GetGridLineStyle();
	void SetGridLineStyle(long nNewValue);
	BOOL GetShowItemsInGroups();
	void SetShowItemsInGroups(BOOL bNewValue);
	BOOL GetShowReadingPane();
	void SetShowReadingPane(BOOL bNewValue);
	BOOL GetHideReadingPaneHeaderInfo();
	void SetHideReadingPaneHeaderInfo(BOOL bNewValue);
	BOOL GetShowUnreadAndFlaggedMessages();
	void SetShowUnreadAndFlaggedMessages(BOOL bNewValue);
	LPDISPATCH GetRowFont();
	LPDISPATCH GetColumnFont();
	LPDISPATCH GetAutoPreviewFont();
	long GetAutoPreview();
	void SetAutoPreview(long nNewValue);
	LPDISPATCH GetAutoFormatRules();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _ViewFields wrapper class

	class _ViewFields : public COleDispatchDriver
	{
	public:
	_ViewFields() {}		// Calls COleDispatchDriver default constructor
	_ViewFields(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_ViewFields(const _ViewFields& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH Add(LPCTSTR PropertyName);
	LPDISPATCH Insert(LPCTSTR PropertyName, const VARIANT& Index);
	void Remove(const VARIANT& Index);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _ViewField wrapper class

	class _ViewField : public COleDispatchDriver
	{
	public:
	_ViewField() {}		// Calls COleDispatchDriver default constructor
	_ViewField(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_ViewField(const _ViewField& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	CString GetViewXMLSchemaName();
	LPDISPATCH GetColumnFormat();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _ColumnFormat wrapper class

	class _ColumnFormat : public COleDispatchDriver
	{
	public:
	_ColumnFormat() {}		// Calls COleDispatchDriver default constructor
	_ColumnFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_ColumnFormat(const _ColumnFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	CString GetLabel();
	void SetLabel(LPCTSTR lpszNewValue);
	long GetFieldFormat();
	void SetFieldFormat(long nNewValue);
	long GetWidth();
	void SetWidth(long nNewValue);
	long GetAlign();
	void SetAlign(long nNewValue);
	long GetFieldType();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _OrderFields wrapper class

	class _OrderFields : public COleDispatchDriver
	{
	public:
	_OrderFields() {}		// Calls COleDispatchDriver default constructor
	_OrderFields(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_OrderFields(const _OrderFields& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH Add(LPCTSTR PropertyName, const VARIANT& IsDescending);
	void Remove(const VARIANT& Index);
	void RemoveAll();
	LPDISPATCH Insert(LPCTSTR PropertyName, const VARIANT& Index, const VARIANT& IsDescending);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _OrderField wrapper class

	class _OrderField : public COleDispatchDriver
	{
	public:
	_OrderField() {}		// Calls COleDispatchDriver default constructor
	_OrderField(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_OrderField(const _OrderField& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	CString GetViewXMLSchemaName();
	BOOL GetIsDescending();
	void SetIsDescending(BOOL bNewValue);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _ViewFont wrapper class

	class _ViewFont : public COleDispatchDriver
	{
	public:
	_ViewFont() {}		// Calls COleDispatchDriver default constructor
	_ViewFont(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_ViewFont(const _ViewFont& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	BOOL GetBold();
	void SetBold(BOOL bNewValue);
	BOOL GetItalic();
	void SetItalic(BOOL bNewValue);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	long GetSize();
	void SetSize(long nNewValue);
	BOOL GetStrikethrough();
	void SetStrikethrough(BOOL bNewValue);
	BOOL GetUnderline();
	void SetUnderline(BOOL bNewValue);
	long GetColor();
	void SetColor(long nNewValue);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _AutoFormatRules wrapper class

	class _AutoFormatRules : public COleDispatchDriver
	{
	public:
	_AutoFormatRules() {}		// Calls COleDispatchDriver default constructor
	_AutoFormatRules(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_AutoFormatRules(const _AutoFormatRules& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH Add(LPCTSTR Name);
	LPDISPATCH Insert(LPCTSTR Name, const VARIANT& Index);
	void Remove(const VARIANT& Index);
	void RemoveAll();
	void Save();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _AutoFormatRule wrapper class

	class _AutoFormatRule : public COleDispatchDriver
	{
	public:
	_AutoFormatRule() {}		// Calls COleDispatchDriver default constructor
	_AutoFormatRule(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_AutoFormatRule(const _AutoFormatRule& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	CString GetFilter();
	void SetFilter(LPCTSTR lpszNewValue);
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	LPDISPATCH GetFont();
	void SetFont(LPDISPATCH newValue);
	BOOL GetStandard();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _IconView wrapper class

	class _IconView : public COleDispatchDriver
	{
	public:
	_IconView() {}		// Calls COleDispatchDriver default constructor
	_IconView(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_IconView(const _IconView& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	void Apply();
	LPDISPATCH Copy(LPCTSTR Name, long SaveOption);
	void Delete();
	void Reset();
	void Save();
	CString GetLanguage();
	void SetLanguage(LPCTSTR lpszNewValue);
	BOOL GetLockUserChanges();
	void SetLockUserChanges(BOOL bNewValue);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	long GetSaveOption();
	BOOL GetStandard();
	long GetViewType();
	CString GetXml();
	void SetXml(LPCTSTR lpszNewValue);
	void GoToDate(DATE Date);
	CString GetFilter();
	void SetFilter(LPCTSTR lpszNewValue);
	LPDISPATCH GetSortFields();
	long GetIconViewType();
	void SetIconViewType(long nNewValue);
	long GetIconPlacement();
	void SetIconPlacement(long nNewValue);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _CardView wrapper class

	class _CardView : public COleDispatchDriver
	{
	public:
	_CardView() {}		// Calls COleDispatchDriver default constructor
	_CardView(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_CardView(const _CardView& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	void Apply();
	LPDISPATCH Copy(LPCTSTR Name, long SaveOption);
	void Delete();
	void Reset();
	void Save();
	CString GetLanguage();
	void SetLanguage(LPCTSTR lpszNewValue);
	BOOL GetLockUserChanges();
	void SetLockUserChanges(BOOL bNewValue);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	long GetSaveOption();
	BOOL GetStandard();
	long GetViewType();
	CString GetXml();
	void SetXml(LPCTSTR lpszNewValue);
	void GoToDate(DATE Date);
	CString GetFilter();
	void SetFilter(LPCTSTR lpszNewValue);
	LPDISPATCH GetSortFields();
	BOOL GetAllowInCellEditing();
	void SetAllowInCellEditing(BOOL bNewValue);
	BOOL GetShowEmptyFields();
	void SetShowEmptyFields(BOOL bNewValue);
	long GetWidth();
	void SetWidth(long nNewValue);
	long GetMultiLineFieldHeight();
	void SetMultiLineFieldHeight(long nNewValue);
	LPDISPATCH GetViewFields();
	LPDISPATCH GetHeadingsFont();
	LPDISPATCH GetBodyFont();
	LPDISPATCH GetAutoFormatRules();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _CalendarView wrapper class

	class _CalendarView : public COleDispatchDriver
	{
	public:
	_CalendarView() {}		// Calls COleDispatchDriver default constructor
	_CalendarView(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_CalendarView(const _CalendarView& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	void Apply();
	LPDISPATCH Copy(LPCTSTR Name, long SaveOption);
	void Delete();
	void Reset();
	void Save();
	CString GetLanguage();
	void SetLanguage(LPCTSTR lpszNewValue);
	BOOL GetLockUserChanges();
	void SetLockUserChanges(BOOL bNewValue);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	long GetSaveOption();
	BOOL GetStandard();
	long GetViewType();
	CString GetXml();
	void SetXml(LPCTSTR lpszNewValue);
	void GoToDate(DATE Date);
	CString GetFilter();
	void SetFilter(LPCTSTR lpszNewValue);
	CString GetStartField();
	void SetStartField(LPCTSTR lpszNewValue);
	CString GetEndField();
	void SetEndField(LPCTSTR lpszNewValue);
	long GetCalendarViewMode();
	void SetCalendarViewMode(long nNewValue);
	long GetDayWeekTimeScale();
	void SetDayWeekTimeScale(long nNewValue);
	BOOL GetMonthShowEndTime();
	void SetMonthShowEndTime(BOOL bNewValue);
	BOOL GetBoldDatesWithItems();
	void SetBoldDatesWithItems(BOOL bNewValue);
	LPDISPATCH GetDayWeekTimeFont();
	LPDISPATCH GetDayWeekFont();
	LPDISPATCH GetMonthFont();
	LPDISPATCH GetAutoFormatRules();
	long GetDaysInMultiDayMode();
	void SetDaysInMultiDayMode(long nNewValue);
	VARIANT GetDisplayedDates();
	BOOL GetBoldSubjects();
	void SetBoldSubjects(BOOL bNewValue);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _TimelineView wrapper class

	class _TimelineView : public COleDispatchDriver
	{
	public:
	_TimelineView() {}		// Calls COleDispatchDriver default constructor
	_TimelineView(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_TimelineView(const _TimelineView& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	void Apply();
	LPDISPATCH Copy(LPCTSTR Name, long SaveOption);
	void Delete();
	void Reset();
	void Save();
	CString GetLanguage();
	void SetLanguage(LPCTSTR lpszNewValue);
	BOOL GetLockUserChanges();
	void SetLockUserChanges(BOOL bNewValue);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	long GetSaveOption();
	BOOL GetStandard();
	long GetViewType();
	CString GetXml();
	void SetXml(LPCTSTR lpszNewValue);
	void GoToDate(DATE Date);
	CString GetFilter();
	void SetFilter(LPCTSTR lpszNewValue);
	CString GetStartField();
	void SetStartField(LPCTSTR lpszNewValue);
	CString GetEndField();
	void SetEndField(LPCTSTR lpszNewValue);
	LPDISPATCH GetGroupByFields();
	long GetDefaultExpandCollapseSetting();
	void SetDefaultExpandCollapseSetting(long nNewValue);
	BOOL GetShowWeekNumbers();
	void SetShowWeekNumbers(BOOL bNewValue);
	long GetMaxLabelWidth();
	void SetMaxLabelWidth(long nNewValue);
	BOOL GetShowLabelWhenViewingByMonth();
	void SetShowLabelWhenViewingByMonth(BOOL bNewValue);
	LPDISPATCH GetUpperScaleFont();
	LPDISPATCH GetLowerScaleFont();
	LPDISPATCH GetItemFont();
	long GetTimelineViewMode();
	void SetTimelineViewMode(long nNewValue);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _MailModule wrapper class

	class _MailModule : public COleDispatchDriver
	{
	public:
	_MailModule() {}		// Calls COleDispatchDriver default constructor
	_MailModule(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_MailModule(const _MailModule& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetNavigationModuleType();
	CString GetName();
	long GetPosition();
	void SetPosition(long nNewValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	LPDISPATCH GetNavigationGroups();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _NavigationGroups wrapper class

	class _NavigationGroups : public COleDispatchDriver
	{
	public:
	_NavigationGroups() {}		// Calls COleDispatchDriver default constructor
	_NavigationGroups(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_NavigationGroups(const _NavigationGroups& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH Create(LPCTSTR GroupDisplayName);
	void Delete(LPDISPATCH Group);
	LPDISPATCH GetDefaultNavigationGroup(long DefaultFolderGroup);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _NavigationGroup wrapper class

	class _NavigationGroup : public COleDispatchDriver
	{
	public:
	_NavigationGroup() {}		// Calls COleDispatchDriver default constructor
	_NavigationGroup(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_NavigationGroup(const _NavigationGroup& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetGroupType();
	LPDISPATCH GetNavigationFolders();
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	long GetPosition();
	void SetPosition(long nNewValue);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _NavigationFolders wrapper class

	class _NavigationFolders : public COleDispatchDriver
	{
	public:
	_NavigationFolders() {}		// Calls COleDispatchDriver default constructor
	_NavigationFolders(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_NavigationFolders(const _NavigationFolders& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH Add(LPDISPATCH Folder);
	void Remove(LPDISPATCH RemovableFolder);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _NavigationFolder wrapper class

	class _NavigationFolder : public COleDispatchDriver
	{
	public:
	_NavigationFolder() {}		// Calls COleDispatchDriver default constructor
	_NavigationFolder(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_NavigationFolder(const _NavigationFolder& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	LPDISPATCH GetFolder();
	BOOL GetIsSelected();
	void SetIsSelected(BOOL bNewValue);
	BOOL GetIsSideBySide();
	void SetIsSideBySide(BOOL bNewValue);
	long GetPosition();
	void SetPosition(long nNewValue);
	BOOL GetIsRemovable();
	CString GetDisplayName();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _CalendarModule wrapper class

	class _CalendarModule : public COleDispatchDriver
	{
	public:
	_CalendarModule() {}		// Calls COleDispatchDriver default constructor
	_CalendarModule(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_CalendarModule(const _CalendarModule& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetNavigationModuleType();
	CString GetName();
	long GetPosition();
	void SetPosition(long nNewValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	LPDISPATCH GetNavigationGroups();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _ContactsModule wrapper class

	class _ContactsModule : public COleDispatchDriver
	{
	public:
	_ContactsModule() {}		// Calls COleDispatchDriver default constructor
	_ContactsModule(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_ContactsModule(const _ContactsModule& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetNavigationModuleType();
	CString GetName();
	long GetPosition();
	void SetPosition(long nNewValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	LPDISPATCH GetNavigationGroups();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _TasksModule wrapper class

	class _TasksModule : public COleDispatchDriver
	{
	public:
	_TasksModule() {}		// Calls COleDispatchDriver default constructor
	_TasksModule(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_TasksModule(const _TasksModule& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetNavigationModuleType();
	CString GetName();
	long GetPosition();
	void SetPosition(long nNewValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	LPDISPATCH GetNavigationGroups();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _JournalModule wrapper class

	class _JournalModule : public COleDispatchDriver
	{
	public:
	_JournalModule() {}		// Calls COleDispatchDriver default constructor
	_JournalModule(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_JournalModule(const _JournalModule& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetNavigationModuleType();
	CString GetName();
	long GetPosition();
	void SetPosition(long nNewValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	LPDISPATCH GetNavigationGroups();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _NotesModule wrapper class

	class _NotesModule : public COleDispatchDriver
	{
	public:
	_NotesModule() {}		// Calls COleDispatchDriver default constructor
	_NotesModule(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_NotesModule(const _NotesModule& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	long GetNavigationModuleType();
	CString GetName();
	long GetPosition();
	void SetPosition(long nNewValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	LPDISPATCH GetNavigationGroups();
	};
	/////////////////////////////////////////////////////////////////////////////
	// NavigationPaneEvents_12 wrapper class

	class NavigationPaneEvents_12 : public COleDispatchDriver
	{
	public:
	NavigationPaneEvents_12() {}		// Calls COleDispatchDriver default constructor
	NavigationPaneEvents_12(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	NavigationPaneEvents_12(const NavigationPaneEvents_12& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	void ModuleSwitch(LPDISPATCH CurrentModule);
	};
	/////////////////////////////////////////////////////////////////////////////
	// NavigationGroupsEvents_12 wrapper class

	class NavigationGroupsEvents_12 : public COleDispatchDriver
	{
	public:
	NavigationGroupsEvents_12() {}		// Calls COleDispatchDriver default constructor
	NavigationGroupsEvents_12(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	NavigationGroupsEvents_12(const NavigationGroupsEvents_12& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	void SelectedChange(LPDISPATCH NavigationFolder);
	void NavigationFolderAdd(LPDISPATCH NavigationFolder);
	void NavigationFolderRemove();
	};
	/////////////////////////////////////////////////////////////////////////////
	// _BusinessCardView wrapper class

	class _BusinessCardView : public COleDispatchDriver
	{
	public:
	_BusinessCardView() {}		// Calls COleDispatchDriver default constructor
	_BusinessCardView(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_BusinessCardView(const _BusinessCardView& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	LPDISPATCH GetApplication();
	long GetClass();
	LPDISPATCH GetSession();
	LPDISPATCH GetParent();
	void Apply();
	LPDISPATCH Copy(LPCTSTR Name, long SaveOption);
	void Delete();
	void Reset();
	void Save();
	CString GetLanguage();
	void SetLanguage(LPCTSTR lpszNewValue);
	BOOL GetLockUserChanges();
	void SetLockUserChanges(BOOL bNewValue);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	long GetSaveOption();
	BOOL GetStandard();
	long GetViewType();
	CString GetXml();
	void SetXml(LPCTSTR lpszNewValue);
	void GoToDate(DATE Date);
	CString GetFilter();
	void SetFilter(LPCTSTR lpszNewValue);
	LPDISPATCH GetHeadingsFont();
	LPDISPATCH GetSortFields();
	long GetCardSize();
	void SetCardSize(long nNewValue);
	};
	/////////////////////////////////////////////////////////////////////////////
	// _FormRegionStartup wrapper class

	class _FormRegionStartup : public COleDispatchDriver
	{
	public:
	_FormRegionStartup() {}		// Calls COleDispatchDriver default constructor
	_FormRegionStartup(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_FormRegionStartup(const _FormRegionStartup& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
	public:

	// Operations
	public:
	VARIANT GetFormRegionStorage(LPCTSTR FormRegionName, LPDISPATCH Item, long LCID, long FormRegionMode, long FormRegionSize);
	void BeforeFormRegionShow(LPDISPATCH FormRegion);
	VARIANT GetFormRegionManifest(LPCTSTR FormRegionName, long LCID);
	VARIANT GetFormRegionIcon(LPCTSTR FormRegionName, long LCID, long Icon);
	};
	*/
}
// namespace OutlookAPI
/*
class _DRecipientControl : public COleDispatchDriver
{
public:
_DRecipientControl() {}		// Calls COleDispatchDriver default constructor
_DRecipientControl(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
_DRecipientControl(const _DRecipientControl& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
// method 'GetEnabled' not emitted because of invalid return type or parameter type
// method 'SetEnabled' not emitted because of invalid return type or parameter type
long GetBackColor();
void SetBackColor(long nNewValue);
long GetForeColor();
void SetForeColor(long nNewValue);
// method 'GetReadOnly' not emitted because of invalid return type or parameter type
// method 'SetReadOnly' not emitted because of invalid return type or parameter type
LPDISPATCH GetFont();
void SetFont(LPDISPATCH newValue);
long GetSpecialEffect();
void SetSpecialEffect(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// _DRecipientControlEvents wrapper class

class _DRecipientControlEvents : public COleDispatchDriver
{
public:
_DRecipientControlEvents() {}		// Calls COleDispatchDriver default constructor
_DRecipientControlEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
_DRecipientControlEvents(const _DRecipientControlEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// _DDocSiteControl wrapper class

class _DDocSiteControl : public COleDispatchDriver
{
public:
_DDocSiteControl() {}		// Calls COleDispatchDriver default constructor
_DDocSiteControl(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
_DDocSiteControl(const _DDocSiteControl& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
// method 'GetReadOnly' not emitted because of invalid return type or parameter type
// method 'SetReadOnly' not emitted because of invalid return type or parameter type
// method 'GetSuppressAttachments' not emitted because of invalid return type or parameter type
// method 'SetSuppressAttachments' not emitted because of invalid return type or parameter type
};
/////////////////////////////////////////////////////////////////////////////
// _DDocSiteControlEvents wrapper class

class _DDocSiteControlEvents : public COleDispatchDriver
{
public:
_DDocSiteControlEvents() {}		// Calls COleDispatchDriver default constructor
_DDocSiteControlEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
_DDocSiteControlEvents(const _DDocSiteControlEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// OlkControl wrapper class

class OlkControl : public COleDispatchDriver
{
public:
OlkControl() {}		// Calls COleDispatchDriver default constructor
OlkControl(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
OlkControl(const OlkControl& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
CString GetItemProperty();
void SetItemProperty(LPCTSTR lpszNewValue);
CString GetControlProperty();
void SetControlProperty(LPCTSTR lpszNewValue);
CString GetPossibleValues();
void SetPossibleValues(LPCTSTR lpszNewValue);
long GetFormat();
void SetFormat(long nNewValue);
BOOL GetEnableAutoLayout();
void SetEnableAutoLayout(BOOL bNewValue);
long GetMinimumWidth();
void SetMinimumWidth(long nNewValue);
long GetMinimumHeight();
void SetMinimumHeight(long nNewValue);
long GetHorizontalLayout();
void SetHorizontalLayout(long nNewValue);
long GetVerticalLayout();
void SetVerticalLayout(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// _OlkTextBox wrapper class

class _OlkTextBox : public COleDispatchDriver
{
public:
_OlkTextBox() {}		// Calls COleDispatchDriver default constructor
_OlkTextBox(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
_OlkTextBox(const _OlkTextBox& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
BOOL GetAutoSize();
void SetAutoSize(BOOL bNewValue);
BOOL GetAutoTab();
void SetAutoTab(BOOL bNewValue);
BOOL GetAutoWordSelect();
void SetAutoWordSelect(BOOL bNewValue);
unsigned long GetBackColor();
void SetBackColor(unsigned long newValue);
long GetBorderStyle();
void SetBorderStyle(long nNewValue);
long GetDragBehavior();
void SetDragBehavior(long nNewValue);
BOOL GetEnabled();
void SetEnabled(BOOL bNewValue);
long GetEnterFieldBehavior();
void SetEnterFieldBehavior(long nNewValue);
BOOL GetEnterKeyBehavior();
void SetEnterKeyBehavior(BOOL bNewValue);
LPDISPATCH GetFont();
unsigned long GetForeColor();
void SetForeColor(unsigned long newValue);
BOOL GetHideSelection();
void SetHideSelection(BOOL bNewValue);
BOOL GetIntegralHeight();
void SetIntegralHeight(BOOL bNewValue);
BOOL GetLocked();
void SetLocked(BOOL bNewValue);
long GetMaxLength();
void SetMaxLength(long nNewValue);
LPDISPATCH GetMouseIcon();
void SetMouseIcon(LPDISPATCH newValue);
long GetMousePointer();
void SetMousePointer(long nNewValue);
BOOL GetMultiLine();
void SetMultiLine(BOOL bNewValue);
CString GetPasswordChar();
void SetPasswordChar(LPCTSTR lpszNewValue);
long GetScrollbars();
void SetScrollbars(long nNewValue);
BOOL GetSelectionMargin();
void SetSelectionMargin(BOOL bNewValue);
BOOL GetTabKeyBehavior();
void SetTabKeyBehavior(BOOL bNewValue);
CString GetText();
void SetText(LPCTSTR lpszNewValue);
long GetTextAlign();
void SetTextAlign(long nNewValue);
VARIANT GetValue();
void SetValue(const VARIANT& newValue);
BOOL GetWordWrap();
void SetWordWrap(BOOL bNewValue);
long GetSelStart();
void SetSelStart(long nNewValue);
long GetSelLength();
void SetSelLength(long nNewValue);
CString GetSelText();
void Cut();
void Copy();
void Paste();
void Clear();
};
/////////////////////////////////////////////////////////////////////////////
// OlkTextBoxEvents wrapper class

class OlkTextBoxEvents : public COleDispatchDriver
{
public:
OlkTextBoxEvents() {}		// Calls COleDispatchDriver default constructor
OlkTextBoxEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
OlkTextBoxEvents(const OlkTextBoxEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
void Click();
void DoubleClick();
void MouseDown(long Button, long Shift, float X, float Y);
void MouseMove(long Button, long Shift, float X, float Y);
void MouseUp(long Button, long Shift, float X, float Y);
void Enter();
void Exit(BOOL* Cancel);
void KeyDown(long* KeyCode, long Shift);
void KeyPress(long* KeyAscii);
void KeyUp(long* KeyCode, long Shift);
void Change();
void AfterUpdate();
void BeforeUpdate(BOOL* Cancel);
};
/////////////////////////////////////////////////////////////////////////////
// _OlkLabel wrapper class

class _OlkLabel : public COleDispatchDriver
{
public:
_OlkLabel() {}		// Calls COleDispatchDriver default constructor
_OlkLabel(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
_OlkLabel(const _OlkLabel& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
CString GetAccelerator();
void SetAccelerator(LPCTSTR lpszNewValue);
BOOL GetAutoSize();
void SetAutoSize(BOOL bNewValue);
unsigned long GetBackColor();
void SetBackColor(unsigned long newValue);
long GetBackStyle();
void SetBackStyle(long nNewValue);
long GetBorderStyle();
void SetBorderStyle(long nNewValue);
CString GetCaption();
void SetCaption(LPCTSTR lpszNewValue);
BOOL GetEnabled();
void SetEnabled(BOOL bNewValue);
LPDISPATCH GetFont();
unsigned long GetForeColor();
void SetForeColor(unsigned long newValue);
LPDISPATCH GetMouseIcon();
void SetMouseIcon(LPDISPATCH newValue);
long GetMousePointer();
void SetMousePointer(long nNewValue);
long GetTextAlign();
void SetTextAlign(long nNewValue);
BOOL GetWordWrap();
void SetWordWrap(BOOL bNewValue);
BOOL GetUseHeaderColor();
void SetUseHeaderColor(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// OlkLabelEvents wrapper class

class OlkLabelEvents : public COleDispatchDriver
{
public:
OlkLabelEvents() {}		// Calls COleDispatchDriver default constructor
OlkLabelEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
OlkLabelEvents(const OlkLabelEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
void Click();
void DoubleClick();
void MouseDown(long Button, long Shift, float X, float Y);
void MouseMove(long Button, long Shift, float X, float Y);
void MouseUp(long Button, long Shift, float X, float Y);
};
/////////////////////////////////////////////////////////////////////////////
// _OlkCommandButton wrapper class

class _OlkCommandButton : public COleDispatchDriver
{
public:
_OlkCommandButton() {}		// Calls COleDispatchDriver default constructor
_OlkCommandButton(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
_OlkCommandButton(const _OlkCommandButton& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
CString GetAccelerator();
void SetAccelerator(LPCTSTR lpszNewValue);
BOOL GetAutoSize();
void SetAutoSize(BOOL bNewValue);
CString GetCaption();
void SetCaption(LPCTSTR lpszNewValue);
BOOL GetEnabled();
void SetEnabled(BOOL bNewValue);
LPDISPATCH GetFont();
LPDISPATCH GetMouseIcon();
void SetMouseIcon(LPDISPATCH newValue);
long GetMousePointer();
void SetMousePointer(long nNewValue);
BOOL GetWordWrap();
void SetWordWrap(BOOL bNewValue);
long GetTextAlign();
void SetTextAlign(long nNewValue);
LPDISPATCH GetPicture();
void SetPicture(LPDISPATCH newValue);
long GetPictureAlignment();
void SetPictureAlignment(long nNewValue);
BOOL GetDisplayDropArrow();
void SetDisplayDropArrow(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// OlkCommandButtonEvents wrapper class

class OlkCommandButtonEvents : public COleDispatchDriver
{
public:
OlkCommandButtonEvents() {}		// Calls COleDispatchDriver default constructor
OlkCommandButtonEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
OlkCommandButtonEvents(const OlkCommandButtonEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
void Click();
void DoubleClick();
void MouseDown(long Button, long Shift, float X, float Y);
void MouseMove(long Button, long Shift, float X, float Y);
void MouseUp(long Button, long Shift, float X, float Y);
void Enter();
void Exit(BOOL* Cancel);
void KeyDown(long* KeyCode, long Shift);
void KeyPress(long* KeyAscii);
void KeyUp(long* KeyCode, long Shift);
void AfterUpdate();
void BeforeUpdate(BOOL* Cancel);
};
/////////////////////////////////////////////////////////////////////////////
// _OlkCheckBox wrapper class

class _OlkCheckBox : public COleDispatchDriver
{
public:
_OlkCheckBox() {}		// Calls COleDispatchDriver default constructor
_OlkCheckBox(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
_OlkCheckBox(const _OlkCheckBox& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
CString GetAccelerator();
void SetAccelerator(LPCTSTR lpszNewValue);
long GetAlignment();
void SetAlignment(long nNewValue);
unsigned long GetBackColor();
void SetBackColor(unsigned long newValue);
long GetBackStyle();
void SetBackStyle(long nNewValue);
CString GetCaption();
void SetCaption(LPCTSTR lpszNewValue);
BOOL GetEnabled();
void SetEnabled(BOOL bNewValue);
LPDISPATCH GetFont();
unsigned long GetForeColor();
void SetForeColor(unsigned long newValue);
LPDISPATCH GetMouseIcon();
void SetMouseIcon(LPDISPATCH newValue);
long GetMousePointer();
void SetMousePointer(long nNewValue);
BOOL GetTripleState();
void SetTripleState(BOOL bNewValue);
VARIANT GetValue();
void SetValue(const VARIANT& newValue);
BOOL GetWordWrap();
void SetWordWrap(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// OlkCheckBoxEvents wrapper class

class OlkCheckBoxEvents : public COleDispatchDriver
{
public:
OlkCheckBoxEvents() {}		// Calls COleDispatchDriver default constructor
OlkCheckBoxEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
OlkCheckBoxEvents(const OlkCheckBoxEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
void Click();
void DoubleClick();
void MouseDown(long Button, long Shift, float X, float Y);
void MouseMove(long Button, long Shift, float X, float Y);
void MouseUp(long Button, long Shift, float X, float Y);
void Enter();
void Exit(BOOL* Cancel);
void KeyDown(long* KeyCode, long Shift);
void KeyPress(long* KeyAscii);
void KeyUp(long* KeyCode, long Shift);
void Change();
void AfterUpdate();
void BeforeUpdate(BOOL* Cancel);
};
/////////////////////////////////////////////////////////////////////////////
// _OlkOptionButton wrapper class

class _OlkOptionButton : public COleDispatchDriver
{
public:
_OlkOptionButton() {}		// Calls COleDispatchDriver default constructor
_OlkOptionButton(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
_OlkOptionButton(const _OlkOptionButton& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
CString GetAccelerator();
void SetAccelerator(LPCTSTR lpszNewValue);
long GetAlignment();
void SetAlignment(long nNewValue);
unsigned long GetBackColor();
void SetBackColor(unsigned long newValue);
long GetBackStyle();
void SetBackStyle(long nNewValue);
CString GetCaption();
void SetCaption(LPCTSTR lpszNewValue);
BOOL GetEnabled();
void SetEnabled(BOOL bNewValue);
LPDISPATCH GetFont();
unsigned long GetForeColor();
void SetForeColor(unsigned long newValue);
CString GetGroupName();
void SetGroupName(LPCTSTR lpszNewValue);
LPDISPATCH GetMouseIcon();
void SetMouseIcon(LPDISPATCH newValue);
long GetMousePointer();
void SetMousePointer(long nNewValue);
VARIANT GetValue();
void SetValue(const VARIANT& newValue);
BOOL GetWordWrap();
void SetWordWrap(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// OlkOptionButtonEvents wrapper class

class OlkOptionButtonEvents : public COleDispatchDriver
{
public:
OlkOptionButtonEvents() {}		// Calls COleDispatchDriver default constructor
OlkOptionButtonEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
OlkOptionButtonEvents(const OlkOptionButtonEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
void Click();
void DoubleClick();
void MouseDown(long Button, long Shift, float X, float Y);
void MouseMove(long Button, long Shift, float X, float Y);
void MouseUp(long Button, long Shift, float X, float Y);
void Enter();
void Exit(BOOL* Cancel);
void KeyDown(long* KeyCode, long Shift);
void KeyPress(long* KeyAscii);
void KeyUp(long* KeyCode, long Shift);
void Change();
void AfterUpdate();
void BeforeUpdate(BOOL* Cancel);
};
/////////////////////////////////////////////////////////////////////////////
// _OlkComboBox wrapper class

class _OlkComboBox : public COleDispatchDriver
{
public:
_OlkComboBox() {}		// Calls COleDispatchDriver default constructor
_OlkComboBox(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
_OlkComboBox(const _OlkComboBox& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
BOOL GetAutoSize();
void SetAutoSize(BOOL bNewValue);
BOOL GetAutoTab();
void SetAutoTab(BOOL bNewValue);
BOOL GetAutoWordSelect();
void SetAutoWordSelect(BOOL bNewValue);
unsigned long GetBackColor();
void SetBackColor(unsigned long newValue);
long GetBorderStyle();
void SetBorderStyle(long nNewValue);
long GetDragBehavior();
void SetDragBehavior(long nNewValue);
BOOL GetEnabled();
void SetEnabled(BOOL bNewValue);
long GetEnterFieldBehavior();
void SetEnterFieldBehavior(long nNewValue);
LPDISPATCH GetFont();
unsigned long GetForeColor();
void SetForeColor(unsigned long newValue);
BOOL GetHideSelection();
void SetHideSelection(BOOL bNewValue);
BOOL GetLocked();
void SetLocked(BOOL bNewValue);
long GetMaxLength();
void SetMaxLength(long nNewValue);
LPDISPATCH GetMouseIcon();
void SetMouseIcon(LPDISPATCH newValue);
long GetMousePointer();
void SetMousePointer(long nNewValue);
BOOL GetSelectionMargin();
void SetSelectionMargin(BOOL bNewValue);
long GetStyle();
void SetStyle(long nNewValue);
CString GetText();
void SetText(LPCTSTR lpszNewValue);
long GetTextAlign();
void SetTextAlign(long nNewValue);
long GetTopIndex();
void SetTopIndex(long nNewValue);
VARIANT GetValue();
void SetValue(const VARIANT& newValue);
long GetListIndex();
void SetListIndex(long nNewValue);
long GetListCount();
long GetSelStart();
void SetSelStart(long nNewValue);
long GetSelLength();
void SetSelLength(long nNewValue);
CString GetSelText();
CString GetItem(long Index);
void SetItem(long Index, LPCTSTR Item);
void Cut();
void Copy();
void Paste();
void Clear();
void AddItem(LPCTSTR ItemText, const VARIANT& Index);
void RemoveItem(long Index);
void DropDown();
};
/////////////////////////////////////////////////////////////////////////////
// OlkComboBoxEvents wrapper class

class OlkComboBoxEvents : public COleDispatchDriver
{
public:
OlkComboBoxEvents() {}		// Calls COleDispatchDriver default constructor
OlkComboBoxEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
OlkComboBoxEvents(const OlkComboBoxEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
void Click();
void DoubleClick();
void MouseDown(long Button, long Shift, float X, float Y);
void MouseMove(long Button, long Shift, float X, float Y);
void MouseUp(long Button, long Shift, float X, float Y);
void Enter();
void Exit(BOOL* Cancel);
void KeyDown(long* KeyCode, long Shift);
void KeyPress(long* KeyAscii);
void KeyUp(long* KeyCode, long Shift);
void Change();
void DropButtonClick();
void AfterUpdate();
void BeforeUpdate(BOOL* Cancel);
};
/////////////////////////////////////////////////////////////////////////////
// _OlkListBox wrapper class

class _OlkListBox : public COleDispatchDriver
{
public:
_OlkListBox() {}		// Calls COleDispatchDriver default constructor
_OlkListBox(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
_OlkListBox(const _OlkListBox& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
long GetBorderStyle();
void SetBorderStyle(long nNewValue);
unsigned long GetBackColor();
void SetBackColor(unsigned long newValue);
BOOL GetEnabled();
void SetEnabled(BOOL bNewValue);
LPDISPATCH GetFont();
unsigned long GetForeColor();
void SetForeColor(unsigned long newValue);
BOOL GetLocked();
void SetLocked(BOOL bNewValue);
long GetMatchEntry();
void SetMatchEntry(long nNewValue);
LPDISPATCH GetMouseIcon();
void SetMouseIcon(LPDISPATCH newValue);
long GetMousePointer();
void SetMousePointer(long nNewValue);
long GetMultiSelect();
void SetMultiSelect(long nNewValue);
CString GetText();
void SetText(LPCTSTR lpszNewValue);
long GetTextAlign();
void SetTextAlign(long nNewValue);
long GetTopIndex();
void SetTopIndex(long nNewValue);
VARIANT GetValue();
void SetValue(const VARIANT& newValue);
long GetListIndex();
void SetListIndex(long nNewValue);
long GetListCount();
CString GetItem(long Index);
void SetItem(long Index, LPCTSTR Item);
BOOL GetSelected(long Index);
void SetSelected(long Index, BOOL Selected);
void Copy();
void Clear();
void AddItem(LPCTSTR ItemText, const VARIANT& Index);
void RemoveItem(long Index);
};
/////////////////////////////////////////////////////////////////////////////
// OlkListBoxEvents wrapper class

class OlkListBoxEvents : public COleDispatchDriver
{
public:
OlkListBoxEvents() {}		// Calls COleDispatchDriver default constructor
OlkListBoxEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
OlkListBoxEvents(const OlkListBoxEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
void Click();
void DoubleClick();
void MouseDown(long Button, long Shift, float X, float Y);
void MouseMove(long Button, long Shift, float X, float Y);
void MouseUp(long Button, long Shift, float X, float Y);
void Enter();
void Exit(BOOL* Cancel);
void KeyDown(long* KeyCode, long Shift);
void KeyPress(long* KeyAscii);
void KeyUp(long* KeyCode, long Shift);
void Change();
void AfterUpdate();
void BeforeUpdate(BOOL* Cancel);
};
/////////////////////////////////////////////////////////////////////////////
// _OlkInfoBar wrapper class

class _OlkInfoBar : public COleDispatchDriver
{
public:
_OlkInfoBar() {}		// Calls COleDispatchDriver default constructor
_OlkInfoBar(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
_OlkInfoBar(const _OlkInfoBar& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
LPDISPATCH GetMouseIcon();
void SetMouseIcon(LPDISPATCH newValue);
long GetMousePointer();
void SetMousePointer(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// OlkInfoBarEvents wrapper class

class OlkInfoBarEvents : public COleDispatchDriver
{
public:
OlkInfoBarEvents() {}		// Calls COleDispatchDriver default constructor
OlkInfoBarEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
OlkInfoBarEvents(const OlkInfoBarEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
void Click();
void DoubleClick();
void MouseDown(long Button, long Shift, float X, float Y);
void MouseMove(long Button, long Shift, float X, float Y);
void MouseUp(long Button, long Shift, float X, float Y);
};
/////////////////////////////////////////////////////////////////////////////
// _OlkContactPhoto wrapper class

class _OlkContactPhoto : public COleDispatchDriver
{
public:
_OlkContactPhoto() {}		// Calls COleDispatchDriver default constructor
_OlkContactPhoto(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
_OlkContactPhoto(const _OlkContactPhoto& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
BOOL GetEnabled();
void SetEnabled(BOOL bNewValue);
LPDISPATCH GetMouseIcon();
void SetMouseIcon(LPDISPATCH newValue);
long GetMousePointer();
void SetMousePointer(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// OlkContactPhotoEvents wrapper class

class OlkContactPhotoEvents : public COleDispatchDriver
{
public:
OlkContactPhotoEvents() {}		// Calls COleDispatchDriver default constructor
OlkContactPhotoEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
OlkContactPhotoEvents(const OlkContactPhotoEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
void Click();
void DoubleClick();
void MouseDown(long Button, long Shift, float X, float Y);
void MouseMove(long Button, long Shift, float X, float Y);
void MouseUp(long Button, long Shift, float X, float Y);
void Enter();
void Exit(BOOL* Cancel);
void KeyDown(long* KeyCode, long Shift);
void KeyPress(long* KeyAscii);
void KeyUp(long* KeyCode, long Shift);
void Change();
};
/////////////////////////////////////////////////////////////////////////////
// _OlkBusinessCardControl wrapper class

class _OlkBusinessCardControl : public COleDispatchDriver
{
public:
_OlkBusinessCardControl() {}		// Calls COleDispatchDriver default constructor
_OlkBusinessCardControl(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
_OlkBusinessCardControl(const _OlkBusinessCardControl& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
LPDISPATCH GetMouseIcon();
void SetMouseIcon(LPDISPATCH newValue);
long GetMousePointer();
void SetMousePointer(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// OlkBusinessCardControlEvents wrapper class

class OlkBusinessCardControlEvents : public COleDispatchDriver
{
public:
OlkBusinessCardControlEvents() {}		// Calls COleDispatchDriver default constructor
OlkBusinessCardControlEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
OlkBusinessCardControlEvents(const OlkBusinessCardControlEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
void Click();
void DoubleClick();
void MouseDown(long Button, long Shift, float X, float Y);
void MouseMove(long Button, long Shift, float X, float Y);
void MouseUp(long Button, long Shift, float X, float Y);
};
/////////////////////////////////////////////////////////////////////////////
// _OlkPageControl wrapper class

class _OlkPageControl : public COleDispatchDriver
{
public:
_OlkPageControl() {}		// Calls COleDispatchDriver default constructor
_OlkPageControl(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
_OlkPageControl(const _OlkPageControl& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
long GetPage();
void SetPage(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// OlkPageControlEvents wrapper class

class OlkPageControlEvents : public COleDispatchDriver
{
public:
OlkPageControlEvents() {}		// Calls COleDispatchDriver default constructor
OlkPageControlEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
OlkPageControlEvents(const OlkPageControlEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// _OlkDateControl wrapper class

class _OlkDateControl : public COleDispatchDriver
{
public:
_OlkDateControl() {}		// Calls COleDispatchDriver default constructor
_OlkDateControl(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
_OlkDateControl(const _OlkDateControl& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
BOOL GetAutoSize();
void SetAutoSize(BOOL bNewValue);
BOOL GetAutoWordSelect();
void SetAutoWordSelect(BOOL bNewValue);
unsigned long GetBackColor();
void SetBackColor(unsigned long newValue);
long GetBackStyle();
void SetBackStyle(long nNewValue);
DATE GetDate();
void SetDate(DATE newValue);
BOOL GetEnabled();
void SetEnabled(BOOL bNewValue);
long GetEnterFieldBehavior();
void SetEnterFieldBehavior(long nNewValue);
LPDISPATCH GetFont();
unsigned long GetForeColor();
void SetForeColor(unsigned long newValue);
BOOL GetHideSelection();
void SetHideSelection(BOOL bNewValue);
BOOL GetLocked();
void SetLocked(BOOL bNewValue);
LPDISPATCH GetMouseIcon();
void SetMouseIcon(LPDISPATCH newValue);
long GetMousePointer();
void SetMousePointer(long nNewValue);
BOOL GetShowNoneButton();
void SetShowNoneButton(BOOL bNewValue);
CString GetText();
void SetText(LPCTSTR lpszNewValue);
long GetTextAlign();
void SetTextAlign(long nNewValue);
VARIANT GetValue();
void SetValue(const VARIANT& newValue);
void DropDown();
};
/////////////////////////////////////////////////////////////////////////////
// OlkDateControlEvents wrapper class

class OlkDateControlEvents : public COleDispatchDriver
{
public:
OlkDateControlEvents() {}		// Calls COleDispatchDriver default constructor
OlkDateControlEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
OlkDateControlEvents(const OlkDateControlEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
void Click();
void DoubleClick();
void MouseDown(long Button, long Shift, float X, float Y);
void MouseMove(long Button, long Shift, float X, float Y);
void MouseUp(long Button, long Shift, float X, float Y);
void Enter();
void Exit(BOOL* Cancel);
void KeyDown(long* KeyCode, long Shift);
void KeyPress(long* KeyAscii);
void KeyUp(long* KeyCode, long Shift);
void Change();
void DropButtonClick();
void AfterUpdate();
void BeforeUpdate(BOOL* Cancel);
};
/////////////////////////////////////////////////////////////////////////////
// _OlkTimeControl wrapper class

class _OlkTimeControl : public COleDispatchDriver
{
public:
_OlkTimeControl() {}		// Calls COleDispatchDriver default constructor
_OlkTimeControl(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
_OlkTimeControl(const _OlkTimeControl& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
BOOL GetAutoSize();
void SetAutoSize(BOOL bNewValue);
BOOL GetAutoWordSelect();
void SetAutoWordSelect(BOOL bNewValue);
unsigned long GetBackColor();
void SetBackColor(unsigned long newValue);
long GetBackStyle();
void SetBackStyle(long nNewValue);
BOOL GetEnabled();
void SetEnabled(BOOL bNewValue);
long GetEnterFieldBehavior();
void SetEnterFieldBehavior(long nNewValue);
LPDISPATCH GetFont();
unsigned long GetForeColor();
void SetForeColor(unsigned long newValue);
BOOL GetHideSelection();
void SetHideSelection(BOOL bNewValue);
BOOL GetLocked();
void SetLocked(BOOL bNewValue);
LPDISPATCH GetMouseIcon();
void SetMouseIcon(LPDISPATCH newValue);
long GetMousePointer();
void SetMousePointer(long nNewValue);
DATE GetReferenceTime();
void SetReferenceTime(DATE newValue);
long GetStyle();
void SetStyle(long nNewValue);
DATE GetTime();
void SetTime(DATE newValue);
CString GetText();
void SetText(LPCTSTR lpszNewValue);
long GetTextAlign();
void SetTextAlign(long nNewValue);
VARIANT GetValue();
void SetValue(const VARIANT& newValue);
DATE GetIntervalTime();
void SetIntervalTime(DATE newValue);
void DropDown();
};
/////////////////////////////////////////////////////////////////////////////
// OlkTimeControlEvents wrapper class

class OlkTimeControlEvents : public COleDispatchDriver
{
public:
OlkTimeControlEvents() {}		// Calls COleDispatchDriver default constructor
OlkTimeControlEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
OlkTimeControlEvents(const OlkTimeControlEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
void Click();
void DoubleClick();
void MouseDown(long Button, long Shift, float X, float Y);
void MouseMove(long Button, long Shift, float X, float Y);
void MouseUp(long Button, long Shift, float X, float Y);
void Enter();
void Exit(BOOL* Cancel);
void KeyDown(long* KeyCode, long Shift);
void KeyPress(long* KeyAscii);
void KeyUp(long* KeyCode, long Shift);
void Change();
void DropButtonClick();
void AfterUpdate();
void BeforeUpdate(BOOL* Cancel);
};
/////////////////////////////////////////////////////////////////////////////
// _OlkCategory wrapper class

class _OlkCategory : public COleDispatchDriver
{
public:
_OlkCategory() {}		// Calls COleDispatchDriver default constructor
_OlkCategory(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
_OlkCategory(const _OlkCategory& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
BOOL GetAutoSize();
void SetAutoSize(BOOL bNewValue);
unsigned long GetBackColor();
void SetBackColor(unsigned long newValue);
long GetBackStyle();
void SetBackStyle(long nNewValue);
BOOL GetEnabled();
void SetEnabled(BOOL bNewValue);
unsigned long GetForeColor();
void SetForeColor(unsigned long newValue);
LPDISPATCH GetMouseIcon();
void SetMouseIcon(LPDISPATCH newValue);
long GetMousePointer();
void SetMousePointer(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// OlkCategoryEvents wrapper class

class OlkCategoryEvents : public COleDispatchDriver
{
public:
OlkCategoryEvents() {}		// Calls COleDispatchDriver default constructor
OlkCategoryEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
OlkCategoryEvents(const OlkCategoryEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
void Click();
void DoubleClick();
void MouseDown(long Button, long Shift, float X, float Y);
void MouseMove(long Button, long Shift, float X, float Y);
void MouseUp(long Button, long Shift, float X, float Y);
void Enter();
void Exit(BOOL* Cancel);
void KeyDown(long* KeyCode, long Shift);
void KeyPress(long* KeyAscii);
void KeyUp(long* KeyCode, long Shift);
void Change();
};
/////////////////////////////////////////////////////////////////////////////
// _OlkFrameHeader wrapper class

class _OlkFrameHeader : public COleDispatchDriver
{
public:
_OlkFrameHeader() {}		// Calls COleDispatchDriver default constructor
_OlkFrameHeader(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
_OlkFrameHeader(const _OlkFrameHeader& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
long GetAlignment();
void SetAlignment(long nNewValue);
CString GetCaption();
void SetCaption(LPCTSTR lpszNewValue);
LPDISPATCH GetFont();
unsigned long GetForeColor();
void SetForeColor(unsigned long newValue);
BOOL GetEnabled();
void SetEnabled(BOOL bNewValue);
LPDISPATCH GetMouseIcon();
void SetMouseIcon(LPDISPATCH newValue);
long GetMousePointer();
void SetMousePointer(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// OlkFrameHeaderEvents wrapper class

class OlkFrameHeaderEvents : public COleDispatchDriver
{
public:
OlkFrameHeaderEvents() {}		// Calls COleDispatchDriver default constructor
OlkFrameHeaderEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
OlkFrameHeaderEvents(const OlkFrameHeaderEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// _OlkSenderPhoto wrapper class

class _OlkSenderPhoto : public COleDispatchDriver
{
public:
_OlkSenderPhoto() {}		// Calls COleDispatchDriver default constructor
_OlkSenderPhoto(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
_OlkSenderPhoto(const _OlkSenderPhoto& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
BOOL GetEnabled();
void SetEnabled(BOOL bNewValue);
LPDISPATCH GetMouseIcon();
void SetMouseIcon(LPDISPATCH newValue);
long GetMousePointer();
void SetMousePointer(long nNewValue);
long GetPreferredWidth();
long GetPreferredHeight();
};
/////////////////////////////////////////////////////////////////////////////
// OlkSenderPhotoEvents wrapper class

class OlkSenderPhotoEvents : public COleDispatchDriver
{
public:
OlkSenderPhotoEvents() {}		// Calls COleDispatchDriver default constructor
OlkSenderPhotoEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
OlkSenderPhotoEvents(const OlkSenderPhotoEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
void Click();
void DoubleClick();
void MouseDown(long Button, long Shift, float X, float Y);
void MouseMove(long Button, long Shift, float X, float Y);
void MouseUp(long Button, long Shift, float X, float Y);
void Change();
};
/////////////////////////////////////////////////////////////////////////////
// _TimeZone wrapper class

class _TimeZone : public COleDispatchDriver
{
public:
_TimeZone() {}		// Calls COleDispatchDriver default constructor
_TimeZone(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
_TimeZone(const _TimeZone& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
LPDISPATCH GetApplication();
long GetClass();
LPDISPATCH GetSession();
LPDISPATCH GetParent();
CString GetName();
CString GetDaylightDesignation();
CString GetStandardDesignation();
long GetBias();
long GetStandardBias();
long GetDaylightBias();
DATE GetStandardDate();
DATE GetDaylightDate();
CString GetId();
};
*/

/*
/////////////////////////////////////////////////////////////////////////////
// Actions wrapper class

class Actions : public COleDispatchDriver
{
public:
Actions() {}		// Calls COleDispatchDriver default constructor
Actions(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
Actions(const Actions& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
LPDISPATCH GetApplication();
long GetClass();
LPDISPATCH GetSession();
LPDISPATCH GetParent();
long GetCount();
LPDISPATCH Item(const VARIANT& Index);
LPDISPATCH Add();
void Remove(long Index);
};
/////////////////////////////////////////////////////////////////////////////
// Action wrapper class

class Action : public COleDispatchDriver
{
public:
Action() {}		// Calls COleDispatchDriver default constructor
Action(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
Action(const Action& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
LPDISPATCH GetApplication();
long GetClass();
LPDISPATCH GetSession();
LPDISPATCH GetParent();
long GetCopyLike();
void SetCopyLike(long nNewValue);
BOOL GetEnabled();
void SetEnabled(BOOL bNewValue);
CString GetMessageClass();
void SetMessageClass(LPCTSTR lpszNewValue);
CString GetName();
void SetName(LPCTSTR lpszNewValue);
CString GetPrefix();
void SetPrefix(LPCTSTR lpszNewValue);
long GetReplyStyle();
void SetReplyStyle(long nNewValue);
long GetResponseStyle();
void SetResponseStyle(long nNewValue);
long GetShowOn();
void SetShowOn(long nNewValue);
void Delete();
LPDISPATCH Execute();
};
*/
/*
/////////////////////////////////////////////////////////////////////////////
// _PropertyAccessor wrapper class

class _PropertyAccessor : public COleDispatchDriver
{
public:
_PropertyAccessor() {}		// Calls COleDispatchDriver default constructor
_PropertyAccessor(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
_PropertyAccessor(const _PropertyAccessor& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
LPDISPATCH GetApplication();
long GetClass();
LPDISPATCH GetSession();
LPDISPATCH GetParent();
VARIANT GetProperty_(LPCTSTR SchemaName);
void SetProperty_(LPCTSTR SchemaName, const VARIANT& Value);
VARIANT GetProperties(const VARIANT& SchemaNames);
VARIANT SetProperties(const VARIANT& SchemaNames, const VARIANT& Values);
DATE UTCToLocalTime(DATE Value);
DATE LocalTimeToUTC(DATE Value);
VARIANT StringToBinary(LPCTSTR Value);
CString BinaryToString(const VARIANT& Value);
void DeleteProperty(LPCTSTR SchemaName);
VARIANT DeleteProperties(VARIANT* SchemaNames);
};
/////////////////////////////////////////////////////////////////////////////
// FormDescription wrapper class

class FormDescription : public COleDispatchDriver
{
public:
FormDescription() {}		// Calls COleDispatchDriver default constructor
FormDescription(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
FormDescription(const FormDescription& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
LPDISPATCH GetApplication();
long GetClass();
LPDISPATCH GetSession();
LPDISPATCH GetParent();
CString GetCategory();
void SetCategory(LPCTSTR lpszNewValue);
CString GetCategorySub();
void SetCategorySub(LPCTSTR lpszNewValue);
CString GetComment();
void SetComment(LPCTSTR lpszNewValue);
CString GetContactName();
void SetContactName(LPCTSTR lpszNewValue);
CString GetDisplayName();
void SetDisplayName(LPCTSTR lpszNewValue);
BOOL GetHidden();
void SetHidden(BOOL bNewValue);
CString GetIcon();
void SetIcon(LPCTSTR lpszNewValue);
BOOL GetLocked();
void SetLocked(BOOL bNewValue);
CString GetMessageClass();
CString GetMiniIcon();
void SetMiniIcon(LPCTSTR lpszNewValue);
CString GetName();
void SetName(LPCTSTR lpszNewValue);
CString GetNumber();
void SetNumber(LPCTSTR lpszNewValue);
BOOL GetOneOff();
void SetOneOff(BOOL bNewValue);
CString GetScriptText();
CString GetTemplate();
void SetTemplate(LPCTSTR lpszNewValue);
BOOL GetUseWordMail();
void SetUseWordMail(BOOL bNewValue);
CString GetVersion();
void SetVersion(LPCTSTR lpszNewValue);
void PublishForm(long Registry, const VARIANT& Folder);
};
/////////////////////////////////////////////////////////////////////////////
// _Inspector wrapper class

class _Inspector : public COleDispatchDriver
{
public:
_Inspector() {}		// Calls COleDispatchDriver default constructor
_Inspector(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
_Inspector(const _Inspector& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
LPDISPATCH GetApplication();
long GetClass();
LPDISPATCH GetSession();
LPDISPATCH GetParent();
LPDISPATCH GetCommandBars();
LPDISPATCH GetCurrentItem();
long GetEditorType();
LPDISPATCH GetModifiedFormPages();
void Close(long SaveMode);
void Display(const VARIANT& Modal);
void HideFormPage(LPCTSTR PageName);
BOOL IsWordMail();
void SetCurrentFormPage(LPCTSTR PageName);
void ShowFormPage(LPCTSTR PageName);
LPDISPATCH GetWordEditor();
CString GetCaption();
long GetHeight();
void SetHeight(long nNewValue);
long GetLeft();
void SetLeft(long nNewValue);
long GetTop();
void SetTop(long nNewValue);
long GetWidth();
void SetWidth(long nNewValue);
long GetWindowState();
void SetWindowState(long nNewValue);
void Activate();
void SetControlItemProperty(LPDISPATCH Control, LPCTSTR PropertyName);
LPDISPATCH NewFormRegion();
LPDISPATCH OpenFormRegion(LPCTSTR Path);
void SaveFormRegion(LPDISPATCH Page, LPCTSTR FileName);
};
/////////////////////////////////////////////////////////////////////////////
// UserProperties wrapper class

class UserProperties : public COleDispatchDriver
{
public:
UserProperties() {}		// Calls COleDispatchDriver default constructor
UserProperties(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
UserProperties(const UserProperties& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
LPDISPATCH GetApplication();
long GetClass();
LPDISPATCH GetSession();
LPDISPATCH GetParent();
long GetCount();
LPDISPATCH Item(const VARIANT& Index);
LPDISPATCH Add(LPCTSTR Name, long Type, const VARIANT& AddToFolderFields, const VARIANT& DisplayFormat);
LPDISPATCH Find(LPCTSTR Name, const VARIANT& Custom);
void Remove(long Index);
};
/////////////////////////////////////////////////////////////////////////////
// UserProperty wrapper class

class UserProperty : public COleDispatchDriver
{
public:
UserProperty() {}		// Calls COleDispatchDriver default constructor
UserProperty(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
UserProperty(const UserProperty& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
LPDISPATCH GetApplication();
long GetClass();
LPDISPATCH GetSession();
LPDISPATCH GetParent();
CString GetFormula();
void SetFormula(LPCTSTR lpszNewValue);
CString GetName();
long GetType();
CString GetValidationFormula();
void SetValidationFormula(LPCTSTR lpszNewValue);
CString GetValidationText();
void SetValidationText(LPCTSTR lpszNewValue);
VARIANT GetValue();
void SetValue(const VARIANT& newValue);
void Delete();
};
*/