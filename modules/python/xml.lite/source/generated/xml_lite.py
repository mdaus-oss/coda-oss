# This file was automatically generated by SWIG (http://www.swig.org).
# Version 4.0.2
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info < (2, 7, 0):
    raise RuntimeError("Python 2.7 or later required")

# Import the low-level C/C++ module
if __package__ or "." in __name__:
    from . import _xml_lite
else:
    import _xml_lite

try:
    import builtins as __builtin__
except ImportError:
    import __builtin__

def _swig_repr(self):
    try:
        strthis = "proxy of " + self.this.__repr__()
    except __builtin__.Exception:
        strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)


def _swig_setattr_nondynamic_instance_variable(set):
    def set_instance_attr(self, name, value):
        if name == "thisown":
            self.this.own(value)
        elif name == "this":
            set(self, name, value)
        elif hasattr(self, name) and isinstance(getattr(type(self), name), property):
            set(self, name, value)
        else:
            raise AttributeError("You cannot add instance attributes to %s" % self)
    return set_instance_attr


def _swig_setattr_nondynamic_class_variable(set):
    def set_class_attr(cls, name, value):
        if hasattr(cls, name) and not isinstance(getattr(cls, name), property):
            set(cls, name, value)
        else:
            raise AttributeError("You cannot add class attributes to %s" % cls)
    return set_class_attr


def _swig_add_metaclass(metaclass):
    """Class decorator for adding a metaclass to a SWIG wrapped class - a slimmed down version of six.add_metaclass"""
    def wrapper(cls):
        return metaclass(cls.__name__, cls.__bases__, cls.__dict__.copy())
    return wrapper


class _SwigNonDynamicMeta(type):
    """Meta class to enforce nondynamic attributes (no new attributes) for a class"""
    __setattr__ = _swig_setattr_nondynamic_class_variable(type.__setattr__)


string_encoding_windows_1252 = _xml_lite.string_encoding_windows_1252

string_encoding_utf_8 = _xml_lite.string_encoding_utf_8

class Element(object):
    r"""Proxy of C++ xml::lite::Element class."""

    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")
    __repr__ = _swig_repr
    __swig_destroy__ = _xml_lite.delete_Element

    def destroyChildren(self) -> "void":
        r"""destroyChildren(Element self)"""
        return _xml_lite.Element_destroyChildren(self)

    def __init__(self, *args):
        r"""
        __init__(Element self) -> Element
        __init__(Element self, std::string const & qname, std::string const & uri="", std::string const & characterData="") -> Element
        __init__(Element self, Element element) -> Element
        """
        _xml_lite.Element_swiginit(self, _xml_lite.new_Element(*args))

    def clone(self, element: "Element") -> "void":
        r"""clone(Element self, Element element)"""
        return _xml_lite.Element_clone(self, element)

    def attribute(self, s: "std::string const &") -> "std::string &":
        r"""attribute(Element self, std::string const & s) -> std::string &"""
        return _xml_lite.Element_attribute(self, s)

    def getElementByTagNameNS(self, *args) -> "xml::lite::Element &":
        r"""
        getElementByTagNameNS(Element self, std::nothrow_t arg2, std::string const & qname, bool recurse=False) -> Element
        getElementByTagNameNS(Element self, std::string const & qname, bool recurse=False) -> Element
        """
        return _xml_lite.Element_getElementByTagNameNS(self, *args)

    def getElementsByTagNameNS(self, *args) -> "std::vector< xml::lite::Element * >":
        r"""
        getElementsByTagNameNS(Element self, std::string const & qname, std::vector< xml::lite::Element * > & elements, bool recurse=False)
        getElementsByTagNameNS(Element self, std::string const & qname, bool recurse=False) -> std::vector< xml::lite::Element * >
        """
        return _xml_lite.Element_getElementsByTagNameNS(self, *args)

    def getElementByTagName(self, *args) -> "xml::lite::Element &":
        r"""
        getElementByTagName(Element self, std::nothrow_t arg2, std::string const & localName, bool recurse=False) -> Element
        getElementByTagName(Element self, std::string const & localName, bool recurse=False) -> Element
        getElementByTagName(Element self, std::nothrow_t arg2, std::string const & uri, std::string const & localName, bool recurse=False) -> Element
        getElementByTagName(Element self, std::string const & uri, std::string const & localName, bool recurse=False) -> Element
        """
        return _xml_lite.Element_getElementByTagName(self, *args)

    def getElementsByTagName(self, *args) -> "std::vector< xml::lite::Element * >":
        r"""
        getElementsByTagName(Element self, std::string const & localName, std::vector< xml::lite::Element * > & elements, bool recurse=False)
        getElementsByTagName(Element self, std::string const & localName, bool recurse=False) -> std::vector< xml::lite::Element * >
        getElementsByTagName(Element self, std::string const & uri, std::string const & localName, std::vector< xml::lite::Element * > & elements, bool recurse=False)
        getElementsByTagName(Element self, std::string const & uri, std::string const & localName, bool recurse=False) -> std::vector< xml::lite::Element * >
        """
        return _xml_lite.Element_getElementsByTagName(self, *args)

    def setNamespacePrefix(self, prefix: "std::string", uri: "std::string") -> "void":
        r"""setNamespacePrefix(Element self, std::string prefix, std::string uri)"""
        return _xml_lite.Element_setNamespacePrefix(self, prefix, uri)

    def setNamespaceURI(self, prefix: "std::string", uri: "std::string") -> "void":
        r"""setNamespaceURI(Element self, std::string prefix, std::string uri)"""
        return _xml_lite.Element_setNamespaceURI(self, prefix, uri)

    def _print(self, *args) -> "void":
        r"""
        _print(Element self, io::OutputStream & stream)
        """
        return _xml_lite.Element__print(self, *args)

    def prettyPrint(self, *args) -> "void":
        r"""
        prettyPrint(Element self, io::OutputStream & stream, std::string const & formatter="    ")
        """
        return _xml_lite.Element_prettyPrint(self, *args)

    def hasElement(self, *args) -> "bool":
        r"""
        hasElement(Element self, std::string const & localName) -> bool
        hasElement(Element self, std::string const & uri, std::string const & localName) -> bool
        """
        return _xml_lite.Element_hasElement(self, *args)

    def getCharacterData(self, *args) -> "void":
        r"""
        getCharacterData(Element self) -> std::string
        """
        return _xml_lite.Element_getCharacterData(self, *args)

    def setCharacterData(self, *args) -> "void":
        r"""
        setCharacterData(Element self, std::string const & characters)
        """
        return _xml_lite.Element_setCharacterData(self, *args)

    def setLocalName(self, localName: "std::string const &") -> "void":
        r"""setLocalName(Element self, std::string const & localName)"""
        return _xml_lite.Element_setLocalName(self, localName)

    def getLocalName(self) -> "std::string":
        r"""getLocalName(Element self) -> std::string"""
        return _xml_lite.Element_getLocalName(self)

    def setQName(self, qname: "std::string const &") -> "void":
        r"""setQName(Element self, std::string const & qname)"""
        return _xml_lite.Element_setQName(self, qname)

    def getQName(self) -> "std::string":
        r"""getQName(Element self) -> std::string"""
        return _xml_lite.Element_getQName(self)

    def setUri(self, uri: "std::string const &") -> "void":
        r"""setUri(Element self, std::string const & uri)"""
        return _xml_lite.Element_setUri(self, uri)

    def getUri(self) -> "std::string":
        r"""getUri(Element self) -> std::string"""
        return _xml_lite.Element_getUri(self)

    def getChildren(self, *args) -> "std::vector< xml::lite::Element * > const &":
        r"""
        getChildren(Element self) -> std::vector< xml::lite::Element * >
        getChildren(Element self) -> std::vector< xml::lite::Element * > const &
        """
        return _xml_lite.Element_getChildren(self, *args)

    def clearChildren(self) -> "void":
        r"""clearChildren(Element self)"""
        return _xml_lite.Element_clearChildren(self)

    def getParent(self) -> "xml::lite::Element *":
        r"""getParent(Element self) -> Element"""
        return _xml_lite.Element_getParent(self)

    def setParent(self, parent: "Element") -> "void":
        r"""setParent(Element self, Element parent)"""
        return _xml_lite.Element_setParent(self, parent)

# Register Element in _xml_lite:
_xml_lite.Element_swigregister(Element)

class Document(object):
    r"""Proxy of C++ xml::lite::Document class."""

    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")
    __repr__ = _swig_repr

    def __init__(self, rootNode: "Element"=None, own: "bool"=True):
        r"""__init__(Document self, Element rootNode=None, bool own=True) -> Document"""
        _xml_lite.Document_swiginit(self, _xml_lite.new_Document(rootNode, own))
    __swig_destroy__ = _xml_lite.delete_Document

    def clone(self) -> "xml::lite::Document *":
        r"""clone(Document self) -> Document"""
        return _xml_lite.Document_clone(self)

    def createElement(self, *args) -> "xml::lite::Element *":
        r"""
        createElement(Document self, std::string const & qname, std::string const & uri, std::string characterData="") -> Element
        """
        return _xml_lite.Document_createElement(self, *args)

    def destroy(self) -> "void":
        r"""destroy(Document self)"""
        return _xml_lite.Document_destroy(self)

    def insert(self, element: "Element", underThis: "Element") -> "void":
        r"""insert(Document self, Element element, Element underThis)"""
        return _xml_lite.Document_insert(self, element, underThis)

    def remove(self, *args) -> "void":
        r"""
        remove(Document self, Element toDelete)
        remove(Document self, Element toDelete, Element fromHere)
        """
        return _xml_lite.Document_remove(self, *args)

    def setRootElement(self, element: "Element", own: "bool"=True) -> "void":
        r"""setRootElement(Document self, Element element, bool own=True)"""
        return _xml_lite.Document_setRootElement(self, element, own)

    def getRootElement(self, *args) -> "xml::lite::Element *":
        r"""
        getRootElement(Document self, bool steal=False) -> Element
        getRootElement(Document self) -> Element
        """
        return _xml_lite.Document_getRootElement(self, *args)

# Register Document in _xml_lite:
_xml_lite.Document_swigregister(Document)

class MinidomParser(object):
    r"""Proxy of C++ xml::lite::MinidomParser class."""

    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")
    __repr__ = _swig_repr

    def __init__(self, storeEncoding: "bool"=True):
        r"""__init__(MinidomParser self, bool storeEncoding=False) -> MinidomParser"""
        _xml_lite.MinidomParser_swiginit(self, _xml_lite.new_MinidomParser(storeEncoding))
    __swig_destroy__ = _xml_lite.delete_MinidomParser

    def parse(self, *args) -> "void":
        r"""parse(MinidomParser self, io::InputStream & _is, int size=io::InputStream::IS_END)"""
        return _xml_lite.MinidomParser_parse(self, *args)

    def clear(self) -> "void":
        r"""clear(MinidomParser self)"""
        return _xml_lite.MinidomParser_clear(self)

    def getDocument(self, *args) -> "xml::lite::Document *":
        r"""
        getDocument(MinidomParser self) -> Document
        getDocument(MinidomParser self, bool steal=False) -> Document
        """
        return _xml_lite.MinidomParser_getDocument(self, *args)

    def getReader(self, *args) -> "XMLReader &":
        r"""
        getReader(MinidomParser self) -> XMLReader const
        getReader(MinidomParser self) -> XMLReader &
        """
        return _xml_lite.MinidomParser_getReader(self, *args)

    def setDocument(self, newDocument: "Document", own: "bool"=True) -> "void":
        r"""setDocument(MinidomParser self, Document newDocument, bool own=True)"""
        return _xml_lite.MinidomParser_setDocument(self, newDocument, own)

    def preserveCharacterData(self, preserve: "bool") -> "void":
        r"""preserveCharacterData(MinidomParser self, bool preserve)"""
        return _xml_lite.MinidomParser_preserveCharacterData(self, preserve)

    def storeEncoding(self, preserve: "bool") -> "void":
        r"""storeEncoding(MinidomParser self, bool preserve)"""
        return _xml_lite.MinidomParser_storeEncoding(self, preserve)

# Register MinidomParser in _xml_lite:
_xml_lite.MinidomParser_swigregister(MinidomParser)



