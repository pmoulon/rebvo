#ifndef KFVO_H
#define KFVO_H

#include "mtracklib/keyframe.h"
namespace rebvo{

class kfvo
{
public:
    kfvo();

    static inline TooN::Vector<3> World2Local(const TooN::Vector<3> &w,const TooN::Matrix <3,3> &Pose,const TooN::Vector<3> &Pos) {
        return Pose.T()*(w-Pos);
    }
    static inline TooN::Vector<3> Local2World(const TooN::Vector<3> &p,const TooN::Matrix <3,3> &Pose,const TooN::Vector<3> &Pos) {
        return Pose*p+Pos;
    }

    static inline TooN::Vector<3> unProject(const TooN::Vector<3> &q,const double &zfm) {
        return TooN::makeVector(q[0]/zfm,q[1]/zfm,1)*(1/q[2]);
    }

    static inline TooN::Vector<3> project(const TooN::Vector<3> &p,const double &zfm) {
        return TooN::makeVector(p[0]*zfm,p[1]*zfm,1)*(1/p[2]);
    }


    static inline TooN::Vector<3>  rotateQ(const TooN::Vector<3> &q0,const TooN::Matrix <3,3> &R,const double &zf){

        using namespace TooN;
        Vector <3> qr=R*makeVector(q0[0]/zf,q0[1]/zf,1);
        Vector <3> q1=Zeros;
        if(fabs(qr[2])>0){

            q1[0]=qr[0]/qr[2]*zf;
            q1[1]=qr[1]/qr[2]*zf;
            q1[2]=q0[2]/qr[2];

        }
        return q1;
    }



    static inline TooN::Vector<4>  rotateQs(const TooN::Vector<4> &q0,const TooN::Matrix <3,3> &R,const double &zf){

        using namespace TooN;
        Vector <3> qr=R*makeVector(q0[0]/zf,q0[1]/zf,1);
        Vector <4> q1=Zeros;
        if(fabs(qr[2])>0){

            q1[0]=qr[0]/qr[2]*zf;
            q1[1]=qr[1]/qr[2]*zf;
            q1[2]=q0[2]/qr[2];
            q1[3]=q0[3]/qr[2];
        }
        return q1;
    }

    static void UpdateRho(edge_tracker &et, TooN::Vector<3> &V);
    static void copyRho(edge_tracker &et);

    static double OptimizePosGT(keyframe &mkf, edge_tracker &et, TooN::Matrix<3, 3> &Pose, TooN::Vector<3> &Pos, double &Kp, int iter_max,double rew_dis,double rho_tol, double s_rho_q, int &mnum,TooN::Vector<6> &X,TooN::Matrix<6> &RRV);

    static double OptimizePos(keyframe &mkf, edge_tracker &et, TooN::Matrix<3, 3> &Pose, TooN::Vector<3> &Pos, const double &K, int iter_max);
    static double OptimizePos1Iter(keyframe &mkf, edge_tracker &et, const TooN::Matrix<3, 3> &BRelRot, const TooN::Vector<3> &BRelPos,TooN::Matrix <6,6> &JtJ,TooN::Vector<6>&JtF);

    static double OptimizeKFPosGT(keyframe &mkf, global_tracker &gt, TooN::Matrix<3, 3> &Pose, TooN::Vector<3> &Pos,double &Kp,int iter_max, double s_rho_q,TooN::Vector<6> &X,TooN::Matrix<6> &RRV);

    static double optimizeScale(keyframe &mkf, edge_tracker &et, const TooN::Matrix<3, 3> &BRelRot, const TooN::Vector<3> &BRelPos);
    static double optimizeScaleBack(keyframe &mkf, edge_tracker &et, const TooN::Matrix<3, 3> &BRelRot, const TooN::Vector<3> &BRelPos, double Kr);

    static double OptimizeRelConstraint1Iter(keyframe &mkf, edge_tracker &et, const TooN::Matrix<3, 3> &BRelRot, const TooN::Vector<3> &BRelPos,TooN::Matrix <6,6> &JtJ,TooN::Vector<6>&JtF,bool etracket_2_keyframe);
    static double OptimizeRelContraint(keyframe &mkf, edge_tracker &et,const TooN::Matrix<3, 3> &Pose, const TooN::Vector<3> &Pos, const double &K, int iter_max,TooN::Vector<6> &X,TooN::Matrix<6, 6> &R_X,bool etracket_2_keyframe);

    static void   countMatches(keyframe &kf_to, keyframe &kf_from, int &kl_on_fov, int &kl_match, float match_mod, float match_ang, float rho_tol, float max_r);

    template <class T,bool ReWeight, bool ProcJF,bool UsePriors>
    static double TryVelRot(TooN::Matrix<6,6,T> &JtJ,TooN::Vector<6,T> &JtF, const TooN::Vector<6,T> &VelRot, const TooN::Vector<3> &V0p, const TooN::Matrix<3,3> &PV0, const TooN::Vector<3> &W0p, const TooN::Matrix<3,3> &PW0,global_tracker &gt, edge_tracker &klist,cam_model &cam_mod,double Kr,T *P0m,int pnum,double match_mod,double match_cang,double rho_tol,double s_rho_min, uint MatchNumThresh,T k_huber,T* DResidual,T* DResidualNew);


    template <class T,bool UsePriors=false>
    static double Minimizer_RV_KF(TooN::Vector<3> &Vel,TooN::Vector<3> &W0,  TooN::Matrix<3,3> &RVel,TooN::Matrix<3,3> &RW0,global_tracker &gt,edge_tracker &klist,cam_model &cam_mod,double Kr,double match_mod,double match_ang,double rho_tol,
                                        int iter_max,double reweigth_distance,const double &max_s_rho,const uint& MatchNumThresh,TooN::Vector<6,T> &X,TooN::Matrix<6> &RRV);

    static int translateDepth(keyframe &mkf, edge_tracker &et, TooN::Matrix<3, 3> &Pose, TooN::Vector<3> &Pos, double &K);
    static int translateDepthBack(keyframe &mkf, edge_tracker &et, TooN::Matrix<3, 3> &Pose, TooN::Vector<3> &Pos, double &K);

    static int matchStereo(keyframe &mkf, edge_tracker &et, TooN::Matrix<3, 3> &Pose, TooN::Vector<3> &Pos,double &K,double mod_thr,double ang_thr,int max_r,double loc_error);



    static int kls_on_fov(keyframe &mkf,TooN::Matrix<3, 3> &Pose, TooN::Vector<3> &Pos);
    static int buildForwardMatch(keyframe &mkf, edge_tracker &et, edge_tracker &et0);

    static void resetForwardMatch(keyframe &mkf);




    static double forwardStereoCorrect(keyframe &mkf,edge_tracker &et,const TooN::Matrix <3,3> &Pose,const TooN::Vector<3> &Pos,double dist_thesh);
    static double forwardStereoCorrect(edge_tracker &et, KeyLine &kl, int &f_id, const TooN::Matrix<3,3> &E, double dist_thesh, const double &zf);

    static int  forwardCorrectAugmentate(keyframe &mkf, edge_tracker &et, const TooN::Matrix<3, 3> &Pose, const TooN::Vector<3> &Pos, double dist_thesh, bool augmentate);
    static void forwardCorrectAugmentate(edge_tracker &et2match, keyframe &mkf, const int &kl_id, const TooN::Matrix<3,3> &E, const double &dist_thesh, const double &zf);


};
}
#endif // KFVO_H
