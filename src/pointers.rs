use crate::ExitCode;

/// Check the pointer is not null and is aligned.
pub(crate) fn check_ptr<T>(ptr: *const T) -> Result<(), ExitCode> {
    if ptr.is_null() {
        return Err(ExitCode::NullPointerError);
    };
    if !ptr.is_aligned() {
        return Err(ExitCode::AlignmentError);
    };
    Ok(())
}

/// Casts a const pointer to a reference. Panics is the pointer is null or not aligned.
///
/// # Safety
///
/// This function requires ``ptr`` to be point to an initialized object of type ``T``.
/// While the resulting reference exists, the memory pointed to must not be mutated.
pub(crate) unsafe fn const_ptr_as_ref<'a, T>(ptr: *const T) -> &'a T {
    check_ptr(ptr).unwrap();
    let as_ref = unsafe { ptr.as_ref() };
    as_ref.unwrap() // we know the pointer is not null, hence we can safely unwrap
}

/// Casts a mut pointer to a mut reference. Panics is the pointer is null or not aligned.
///
/// # Safety
///
/// This function requires ``ptr`` to be point to an initialized object of type ``T``.
/// While the resulting reference exists, the memory pointed to must not be accessed otherwise.
pub(crate) unsafe fn mut_ptr_as_ref<'a, T>(ptr: *mut T) -> &'a mut T {
    check_ptr(ptr).unwrap();
    let as_mut_ref = unsafe { ptr.as_mut() };
    as_mut_ref.unwrap() // we know the pointer is not null, hence we can safely unwrap
}
