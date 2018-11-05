#include "../OSGCommon.h"
#include "PrimitiveRenderer.h"
#include "StopWatch.h"

PrimitiveRenderer* pvd = nullptr;
//string g_pickedObejctName = "";
ref_ptr<Group> g_pRootNode = nullptr;

StopWatch g_stopWatch;

class KeyboardEventHandler : public osgGA::GUIEventHandler
{
public:

	KeyboardEventHandler() {}

	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&)
	{
		switch (ea.getEventType())
		{
		case(osgGA::GUIEventAdapter::KEYDOWN) :
		{
			switch (ea.getKey())
			{
			case 'm':
				printf("M\n");
				return true;
			case 'n':
				printf("M\n");
				return true;
			case 'r':
				printf("R\n");
				return false;
			case 'w':
				printf("W\n");
				return true;
			case 'l':
				printf("L\n");
				return true;
			case 'f':
				printf("F\n");
				return true;
			case '1':
				return true;
			case '2':
				return true;
			case 'z':
				printf("Z\n");
				return false;
			case 'x':
				printf("X\n");
				return false;
			case 'c':
				printf("C\n");
				return false;
			case 'b':
				printf("B\n");
				return false;
			case 's':
				printf("S\n");
				return false;
			case 'a':
				printf("A\n");
				return false;
			default:
				break;

			}
		}
		}
		//return false to allow mouse manipulation
		return false;
	}
};

class MouseEventHandler : public osgGA::GUIEventHandler
{
public:

	MouseEventHandler() {}

	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&)
	{
		switch (ea.getEventType())
		{
		case(osgGA::GUIEventAdapter::PUSH) :
		{
			//if (ea.getButtonMask() | osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
			if (ea.getButtonMask() | osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
			{
				printf("Clicked...!!!\n");
			}
			return true;
		}
			break;
		default:
			break;
		}
		return false;
	}
};

class PickHandler : public osgGA::GUIEventHandler
{
public:
	virtual void PickingResult(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osgUtil::LineSegmentIntersector::Intersection& result)
	{
		osg::Node* node = 0;
		osg::Group* parent = 0;

		osg::NodePath& nodePath = result.nodePath;
		node = (nodePath.size() >= 1) ? nodePath[nodePath.size() - 1] : 0;
		parent = (nodePath.size() >= 2) ? dynamic_cast<osg::Group*>(nodePath[nodePath.size() - 2]) : 0;
		if (parent && node)
		{
			//printf("================================================================\n");
			//for (auto& i : result.indexList)
			//{
			//	printf("picked index %d\n", i);
			//}
			//printf("================================================================\n");

			auto name = result.drawable->asGeometry()->getName();

			auto va = (Vec3Array*)result.drawable->asGeometry()->getVertexArray();
			auto& v0 = va->at(result.indexList.at(0));
			auto& v1 = va->at(result.indexList.at(1));
			auto& v2 = va->at(result.indexList.at(2));

			printf("Picked Obejct Name %s\n", name.c_str());

			if (ea.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_CTRL)
			{
			}
			else if (ea.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_SHIFT)
			{
			}
			else if (ea.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_ALT)
			{
			}
			else
			{
			}
		}
	}

	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
	{
		//if (ea.getEventType() != osgGA::GUIEventAdapter::RELEASE || ea.getButton() != osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON || !(ea.getModKeyMask()&osgGA::GUIEventAdapter::MODKEY_CTRL))
		//	return false;

		if (ea.getEventType() == osgGA::GUIEventAdapter::RELEASE && ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
		{
			osgViewer::View* viewer = dynamic_cast<osgViewer::View*>(&aa);
			if (viewer)
			{
				osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector = new osgUtil::LineSegmentIntersector(osgUtil::Intersector::WINDOW, ea.getX(), ea.getY());
				osgUtil::IntersectionVisitor iv(intersector.get());
				viewer->getCamera()->accept(iv);
				if (intersector->containsIntersections())
				{
					auto result = *(intersector->getIntersections().begin());
					PickingResult(ea, aa, result);
				}
				else
				{
					//g_pickedObejctName = "";
					//printf("g_pickedObejctName %s\n", g_pickedObejctName.c_str());
				}
			}
			return false;
		}
		return false;
	}
};

int main(int argc, char** argv)
{
	osgViewer::Viewer viewer;
	viewer.setUpViewOnSingleScreen(0);
	viewer.getCamera()->setClearColor(Vec4(0.3f, 0.5f, 0.7f, 1.0f));
	//viewer.setUpViewInWindow(300, 300, 800, 600, 1);
	viewer.setUpViewInWindow(300, 300, 800, 600, 0);

	g_pRootNode = new Group;

	viewer.addEventHandler(new KeyboardEventHandler);
	viewer.addEventHandler(new PickHandler);

	// run optimization over the scene graph
	osgUtil::Optimizer optimzer;
	optimzer.optimize(g_pRootNode);

	PrimitiveRenderer vd(g_pRootNode);
	pvd = &vd;

//	// add a viewport to the viewer and attach the scene graph.
	viewer.setSceneData(g_pRootNode);

	StopWatch sw;
	sw.Start();

	auto swt = sw.Touch();
	printf("Loading Takes %f seconds\n", swt.first);

	auto swr = sw.Stop();
	printf("%f seconds\n", swr.second);

	return viewer.run();
}
