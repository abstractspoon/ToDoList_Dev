using System;
using System.Runtime.InteropServices;

namespace System.Windows.Forms.Html {
	[
	ComVisible(true), 
	Guid(@"3050F1FF-98B5-11CF-BB82-00AA00BDCE0B"), 
	InterfaceType(ComInterfaceType.InterfaceIsDual)
	]
	public interface IHTMLElement {
		void SetAttribute(string strAttributeName, object AttributeValue, int lFlags); 
        
		void GetAttribute(string strAttributeName, int lFlags, object[] pvars); 
        
		[return: MarshalAs(UnmanagedType.Bool)] 
		bool RemoveAttribute(string strAttributeName, int lFlags); 
        
		void SetClassName(string p); 
        
		[return: MarshalAs(UnmanagedType.BStr)] 
		string GetClassName(); 
        
		void SetId(string p); 
        
		[return: MarshalAs(UnmanagedType.BStr)] 
		string GetId(); 
        
		[return: MarshalAs(UnmanagedType.BStr)] 
		string GetTagName(); 
        
		[return: MarshalAs(UnmanagedType.Interface)] 
		IHTMLElement GetParentElement(); 
        
		[return: MarshalAs(UnmanagedType.Interface)] 
		object GetStyle(); 
        
		void SetOnhelp(object p); 
        
		[return: MarshalAs(UnmanagedType.Struct)] 
		object GetOnhelp(); 
        
		void SetOnclick(object p); 
        
		[return: MarshalAs(UnmanagedType.Struct)] 
		object GetOnclick(); 
        
		void SetOndblclick(object p); 
        
		[return: MarshalAs(UnmanagedType.Struct)] 
		object GetOndblclick(); 
        
		void SetOnkeydown(object p); 
        
		[return: MarshalAs(UnmanagedType.Struct)] 
		object GetOnkeydown(); 
        
		void SetOnkeyup(object p); 
        
		[return: MarshalAs(UnmanagedType.Struct)] 
		object GetOnkeyup(); 
        
		void SetOnkeypress(object p); 
        
		[return: MarshalAs(UnmanagedType.Struct)] 
		object GetOnkeypress(); 
        
		void SetOnmouseout(object p); 
        
		[return: MarshalAs(UnmanagedType.Struct)] 
		object GetOnmouseout(); 
        
		void SetOnmouseover(object p); 
        
		[return: MarshalAs(UnmanagedType.Struct)] 
		object GetOnmouseover(); 
        
		void SetOnmousemove(object p); 
        
		[return: MarshalAs(UnmanagedType.Struct)] 
		object GetOnmousemove(); 
        
		void SetOnmousedown(object p); 
        
		[return: MarshalAs(UnmanagedType.Struct)] 
		object GetOnmousedown(); 
        
		void SetOnmouseup(object p); 
        
		[return: MarshalAs(UnmanagedType.Struct)] 
		object GetOnmouseup(); 
        
		[return: MarshalAs(UnmanagedType.Interface)] 
		object GetDocument(); 
        
		void SetTitle(string p); 
        
		[return: MarshalAs(UnmanagedType.BStr)] 
		string GetTitle(); 
        
		void SetLanguage(string p); 
        
		[return: MarshalAs(UnmanagedType.BStr)] 
		string GetLanguage(); 
        
		void SetOnselectstart(object p); 
        
		[return: MarshalAs(UnmanagedType.Struct)] 
		object GetOnselectstart(); 
        
		void ScrollIntoView(object varargStart); 
        
		[return: MarshalAs(UnmanagedType.Bool)] 
		bool Contains(IHTMLElement pChild); 
        
		[return: MarshalAs(UnmanagedType.I4)] 
		int GetSourceIndex(); 
        
		[return: MarshalAs(UnmanagedType.Struct)] 
		object GetRecordNumber(); 
        
		void SetLang(string p); 
        
		[return: MarshalAs(UnmanagedType.BStr)] 
		string GetLang(); 
        
		[return: MarshalAs(UnmanagedType.I4)] 
		int GetOffsetLeft(); 
        
		[return: MarshalAs(UnmanagedType.I4)] 
		int GetOffsetTop(); 
        
		[return: MarshalAs(UnmanagedType.I4)] 
		int GetOffsetWidth(); 
        
		[return: MarshalAs(UnmanagedType.I4)] 
		int GetOffsetHeight(); 
        
		[return: MarshalAs(UnmanagedType.Interface)] 
		IHTMLElement GetOffsetParent(); 
        
		void SetInnerHTML(string p); 
        
		[return: MarshalAs(UnmanagedType.BStr)] 
		string GetInnerHTML(); 
        
		void SetInnerText(string p); 
        
		[return: MarshalAs(UnmanagedType.BStr)] 
		string GetInnerText(); 
        
		void SetOuterHTML(string p); 
        
		[return: MarshalAs(UnmanagedType.BStr)] 
		string GetOuterHTML(); 
        
		void SetOuterText(string p); 
        
		[return: MarshalAs(UnmanagedType.BStr)] 
		string GetOuterText(); 
        
		void InsertAdjacentHTML(string where, string html); 
        
		void InsertAdjacentText(string where, string text); 
        
		[return: MarshalAs(UnmanagedType.Interface)] 
		IHTMLElement GetParentTextEdit(); 
        
		[return: MarshalAs(UnmanagedType.Bool)] 
		bool GetIsTextEdit(); 
        
		void Click(); 
        
		[return: MarshalAs(UnmanagedType.Interface)] 
		object GetFilters(); 
        
		void SetOndragstart(object p); 
        
		[return: MarshalAs(UnmanagedType.Struct)] 
		object GetOndragstart(); 
        
		[return: MarshalAs(UnmanagedType.BStr)] 
		string toString(); 
        
		void SetOnbeforeupdate(object p); 
        
		[return: MarshalAs(UnmanagedType.Struct)] 
		object GetOnbeforeupdate(); 
        
		void SetOnafterupdate(object p); 
        
		[return: MarshalAs(UnmanagedType.Struct)] 
		object GetOnafterupdate(); 
        
		void SetOnerrorupdate(object p); 
        
		[return: MarshalAs(UnmanagedType.Struct)] 
		object GetOnerrorupdate(); 
        
		void SetOnrowexit(object p); 
        
		[return: MarshalAs(UnmanagedType.Struct)] 
		object GetOnrowexit(); 
        
		void SetOnrowenter(object p); 
        
		[return: MarshalAs(UnmanagedType.Struct)] 
		object GetOnrowenter(); 
        
		void SetOndatasetchanged(object p); 
        
		[return: MarshalAs(UnmanagedType.Struct)] 
		object GetOndatasetchanged(); 
        
		void SetOndataavailable(object p); 
        
		[return: MarshalAs(UnmanagedType.Struct)] 
		object GetOndataavailable(); 
        
		void SetOndatasetcomplete(object p); 
        
		[return: MarshalAs(UnmanagedType.Struct)] 
		object GetOndatasetcomplete(); 
        
		void SetOnfilterchange(object p); 
        
		[return: MarshalAs(UnmanagedType.Struct)] 
		object GetOnfilterchange(); 
        
		[return: MarshalAs(UnmanagedType.Interface)] 
		object GetChildren(); 
        
		[return: MarshalAs(UnmanagedType.Interface)] 
		object GetAll(); 
	} 

	[
		ComVisible(true), 
		Guid(@"3050F434-98B5-11CF-BB82-00AA00BDCE0B"), 
		InterfaceType(ComInterfaceType.InterfaceIsDual)
	]
	public interface IHTMLElement2: IHTMLElement {
		
		[return: MarshalAs(UnmanagedType.BStr)] 
		string scopeName();

		void setCapture(bool containerCapture);

		void releaseCapture();

		void setOnlosecapture(object v);

		[return: MarshalAs(UnmanagedType.Struct)] 
		object getOnlosecapture();

		[return: MarshalAs(UnmanagedType.BStr)] 
		string componentFromPoint(int x,int y);

		void doScroll(object component); //maybe better UnmanagedType.BStr?

		void setOnscroll(object v);

		[return: MarshalAs(UnmanagedType.Struct)] 
		object getOnscroll();

		void setOndrag(object v);
		
		[return: MarshalAs(UnmanagedType.Struct)] 
		object getOndrag();

		void setOndragend(object v);

		[return: MarshalAs(UnmanagedType.Struct)] 
		object getOndragend();

		void setOndragenter(object v);

		[return: MarshalAs(UnmanagedType.Struct)] 
		object getOndragenter();

		void setOndragover(object v);

		[return: MarshalAs(UnmanagedType.Struct)] 
		object getOndragover();

		void setOndragleave(object v);

		[return: MarshalAs(UnmanagedType.Struct)] 
		object getOndragleave();

		void setOndrop(object v);

		[return: MarshalAs(UnmanagedType.Struct)] 
		object getOndrop();

		void setOnbeforecut(object v);

		[return: MarshalAs(UnmanagedType.Struct)] 
		object getOnbeforecut();

		void setOncut(object v);

		[return: MarshalAs(UnmanagedType.Struct)] 
		object getOncut();

		void setOnbeforecopy(object v);

		[return: MarshalAs(UnmanagedType.Struct)] 
		object getOnbeforecopy();

		void setOncopy(object v);

		[return: MarshalAs(UnmanagedType.Struct)] 
		object getOncopy();

		void setOnbeforepaste(object v);

		[return: MarshalAs(UnmanagedType.Struct)] 
		object getOnbeforepaste();

		void setOnpaste(object v);

		[return: MarshalAs(UnmanagedType.Struct)] 
		object getOnpaste();

		[return: MarshalAs(UnmanagedType.Interface)] 
		object currentStyle();

		void setOnpropertychange(object v);

		[return: MarshalAs(UnmanagedType.Struct)] 
		object getOnpropertychange();

		[return: MarshalAs(UnmanagedType.Interface)] 
		object getClientRects();

		[return: MarshalAs(UnmanagedType.Interface)] 
		object getBoundingClientRect();

		void setExpression(string propname, string expression, string language);
		
		[return: MarshalAs(UnmanagedType.Struct)] 
		object getExpression(string propname);

		[return: MarshalAs(UnmanagedType.Bool)] 
		bool removeExpression(string propname);

		void setTabIndex(Int16 v);
		Int16 getTabIndex();
		
		void focus();

		void setAccessKey(string v);

		[return: MarshalAs(UnmanagedType.BStr)] 
		string getAccessKey();

		void setOnblur(object v);

		[return: MarshalAs(UnmanagedType.Struct)] 
		object getOnblur();

		void setOnfocus(object v);

		[return: MarshalAs(UnmanagedType.Struct)] 
		object getOnfocus();

		void setOnresize(object v);

		[return: MarshalAs(UnmanagedType.Struct)] 
		object getOnresize();

		void blur();

		void addFilter(object pUnk);

		void  removeFilter(object pUnk);

		int clientHeight();
		int clientWidth();
		int clientTop();
		int clientLeft();

		[return: MarshalAs(UnmanagedType.Bool)] 
		bool attachEvent(string dhtmlEvent,object pDisp);

		void detachEvent(string dhtmlEvent,object pDisp);

		[return: MarshalAs(UnmanagedType.Struct)] 
		object readyState();

		void setOnreadystatechange(object v);

		[return: MarshalAs(UnmanagedType.Struct)] 
		object getOnreadystatechange();

		void setOnrowsdelete(object v);

		[return: MarshalAs(UnmanagedType.Struct)] 
		object getOnrowsdelete();

		void setOnrowsinserted(object v);

		[return: MarshalAs(UnmanagedType.Struct)] 
		object getOnrowsinserted();

		void setOncellchange(object v);

		[return: MarshalAs(UnmanagedType.Struct)] 
		object getOncellchange();

        void setDir(string v);

		[return: MarshalAs(UnmanagedType.BStr)] 
		string getDir();

		[return: MarshalAs(UnmanagedType.Interface)] 
		object createControlRange();

		int scrollHeight();
		int scrollWidth();
		
		void setScrollTop(int v);
		int getScrollTop();

		void setScrollLeft(int v);
		int getScrollLeft();

		void clearAttributes();

	/*
	 *  under construction:
	 * 
		[id(DISPID_IHTMLELEMENT2_MERGEATTRIBUTES)] HRESULT mergeAttributes([in] IHTMLElement* mergeThis);
		[propput, id(DISPID_IHTMLELEMENT2_ONCONTEXTMENU), displaybind, bindable] HRESULT oncontextmenu(object v);
		[propget, id(DISPID_IHTMLELEMENT2_ONCONTEXTMENU), displaybind, bindable] HRESULT oncontextmenu();
		[id(DISPID_IHTMLELEMENT2_INSERTADJACENTELEMENT)] HRESULT insertAdjacentElement([in] BSTR where,[in] IHTMLElement* insertedElement,[retval, out] IHTMLElement** inserted);
		[id(DISPID_IHTMLELEMENT2_APPLYELEMENT)] HRESULT applyElement([in] IHTMLElement* apply,[in] BSTR where,[retval, out] IHTMLElement** applied);
		[id(DISPID_IHTMLELEMENT2_GETADJACENTTEXT)] HRESULT getAdjacentText([in] BSTR where,[retval, out] BSTR* text);
		[id(DISPID_IHTMLELEMENT2_REPLACEADJACENTTEXT)] HRESULT replaceAdjacentText([in] BSTR where,[in] BSTR newText,[retval, out] BSTR* oldText);
		[propget, id(DISPID_IHTMLELEMENT2_CANHAVECHILDREN)] HRESULT canHaveChildren([retval, out] VARIANT_BOOL * p);
		[id(DISPID_IHTMLELEMENT2_ADDBEHAVIOR)] HRESULT addBehavior([in] BSTR bstrUrl,[optional, in] VARIANT* pvarFactory,[retval, out] long* pCookie);
		[id(DISPID_IHTMLELEMENT2_REMOVEBEHAVIOR)] HRESULT removeBehavior([in] long cookie,[retval, out] VARIANT_BOOL* pfResult);
		[propget, id(DISPID_IHTMLELEMENT2_RUNTIMESTYLE), nonbrowsable] HRESULT runtimeStyle([retval, out] IHTMLStyle* * p);
		[propget, id(DISPID_IHTMLELEMENT2_BEHAVIORURNS)] HRESULT behaviorUrns([retval, out] IDispatch* * p);
		[propput, id(DISPID_IHTMLELEMENT2_TAGURN)] HRESULT tagUrn([in] BSTR v);
		[propget, id(DISPID_IHTMLELEMENT2_TAGURN)] HRESULT tagUrn([retval, out] BSTR * p);
		[propput, id(DISPID_IHTMLELEMENT2_ONBEFOREEDITFOCUS), displaybind, bindable] HRESULT onbeforeeditfocus(object v);
		[propget, id(DISPID_IHTMLELEMENT2_ONBEFOREEDITFOCUS), displaybind, bindable] HRESULT onbeforeeditfocus();
		[propget, id(DISPID_IHTMLELEMENT2_READYSTATEVALUE), hidden, restricted] HRESULT readyStateValue([retval, out] long * p);
		[id(DISPID_IHTMLELEMENT2_GETELEMENTSBYTAGNAME)] HRESULT getElementsByTagName([in] BSTR v,[retval, out] IHTMLElementCollection** pelColl);
		*/
	} 
}

